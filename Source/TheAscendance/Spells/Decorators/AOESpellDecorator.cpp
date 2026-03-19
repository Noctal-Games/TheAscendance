// Fill out your copyright notice in the Description page of Project Settings.


#include "AOESpellDecorator.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Spells/Interfaces/SpellCaster.h"
#include "TheAscendance/Characters/Interfaces/Susceptible.h"

#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

void UAOESpellDecorator::LoadHitNiagara()
{
	m_DecoratedSpell->LoadHitNiagara();

	if (m_ModifierData == nullptr)
	{
		LOG_ERROR("[AOE SPELL DECORATOR] Modifier data was invalid");
		return;
	}

	m_AOEHitNiagara = m_ModifierData->AOEHitNiagara;

	if (m_AOEHitNiagara.IsNull() == true)
	{
		LOG_ERROR("[AOE SPELL DECORATOR] Tried to Load AOEHitNiagara with invalid AOEHitNiagara");
		return;
	}

	UCoreFunctionLibrary::RequestAsyncLoad(m_AOEHitNiagara.ToSoftObjectPath());
}

void UAOESpellDecorator::OnHit(AActor* hitActor, const FVector& spellHitLocation)
{
	m_DecoratedSpell->OnHit(hitActor, spellHitLocation);

	if (m_ModifierData == nullptr)
	{
		LOG_ERROR("[AOE SPELL DECORATOR] Modifier data was invalid")
		return;
	}

	if (hitActor != nullptr)
	{
		int damage = 0;
		m_DecoratedSpell->ProcessHitDamage(damage, hitActor->GetActorLocation(), spellHitLocation);
		m_DecoratedSpell->DealDamage(hitActor, damage);
	}

	AActor* owner = GetSpellOwner()->GetActor();

	TArray<TEnumAsByte<EObjectTypeQuery>> types;
	types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
	types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<TObjectPtr<AActor>> ignore;
	//ignore.Add(owner);

	TArray<AActor*> targets;

	UCoreFunctionLibrary::DrawDebugSphere(spellHitLocation, m_ModifierData->Range, 16, FColor::Yellow);

	if (UKismetSystemLibrary::SphereOverlapActors(owner->GetWorld(), spellHitLocation, m_ModifierData->Range, types, NULL, ignore, targets))
	{
		for (TObjectPtr<AActor> a : targets)
		{
			m_DecoratedSpell->OnHit(a, spellHitLocation);
		}
	}
}

void UAOESpellDecorator::ProcessHit(const FVector& spellHitLocation)
{
	if (m_ModifierData->DoesKnockback == false)
	{
		m_DecoratedSpell->ProcessHit(spellHitLocation);
		return;
	}

	for (AActor* actor : GetHitActors())
	{
		FVector knockbackDirection = actor->GetActorLocation() - spellHitLocation;
		knockbackDirection.Normalize();
		knockbackDirection.Z = FMath::Max(knockbackDirection.Z, 0.5f);

		float distance = FVector::Distance(actor->GetActorLocation(), spellHitLocation);
		float normalizedDistance = distance / m_ModifierData->Range;

		float knockbackStrength = m_ModifierData->KnockbackStrength - (m_ModifierData->KnockbackStrength - 0.0f) * normalizedDistance * normalizedDistance;
		knockbackStrength = FMath::Clamp(knockbackStrength, 0.0f, m_ModifierData->KnockbackStrength);

		if (ACharacter* character = Cast<ACharacter>(actor))
		{
			character->LaunchCharacter(knockbackDirection * knockbackStrength, true, true);
			continue;
		}

		if (UPrimitiveComponent* primitiveComponent = actor->FindComponentByClass<UPrimitiveComponent>())
		{
			if (primitiveComponent->IsSimulatingPhysics() == false)
			{
				continue;
			}

			primitiveComponent->AddImpulse(knockbackDirection * knockbackStrength, NAME_None, true);
		}
	}

	m_DecoratedSpell->ProcessHit(spellHitLocation);
}

void UAOESpellDecorator::ProcessHitDamage(int& damage, const FVector& targetLocation, const FVector& hitLocation)
{
	if (m_ModifierData->HasDamageFallOff == false)
	{
		damage += m_ModifierData->Damage;
		return;
	}

	float distance = FVector::Distance(targetLocation, hitLocation);

	float normalizedDistance = distance / m_ModifierData->Range;
	float damageWithFalloff = m_ModifierData->Damage - (m_ModifierData->Damage - m_ModifierData->DamageMinimum) * normalizedDistance * normalizedDistance;

	damageWithFalloff = FMath::Clamp(damageWithFalloff, m_ModifierData->DamageMinimum, m_ModifierData->Damage);

	damage += damageWithFalloff;
}

void UAOESpellDecorator::SpawnHitNiagara(const FVector& spellHitLocation)
{
	m_DecoratedSpell->SpawnHitNiagara(spellHitLocation);

	if (m_AOEHitNiagara.IsValid() == false)
	{
		return;
	}

	if (UWorld* worldContext = UCoreFunctionLibrary::GetGameWorld())
	{
		UNiagaraComponent* vfx = UNiagaraFunctionLibrary::SpawnSystemAtLocation(worldContext, m_AOEHitNiagara.Get(), spellHitLocation);
		vfx->SetVariableFloat(FName("VFX_ShapeScale"), m_ModifierData->Range);
	}
}
