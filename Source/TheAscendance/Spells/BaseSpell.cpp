// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSpell.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "Interfaces/SpellCaster.h"
#include "Structs/SpellData.h"

#include "NiagaraFunctionLibrary.h"

void UBaseSpell::Init(USpellData* spellData, ISpellCaster* spellOwner)
{
	if (spellData == nullptr || spellOwner == nullptr)
	{
		LOG_ERROR("Tried to Init spell with invalid SpellData or SpellOwner");
		return;
	}

	m_SpellOwner = spellOwner->_getUObject();
	m_SpellNiagara = spellData->SpellNiagara;

	if(m_SpellNiagara.IsNull() == true)
	{
		LOG_ERROR("Tried to Init spell with invalid SpellNiagara");
		return;
	}

	m_Cooldown = spellData->SpellCooldown;

	UCoreFunctionLibrary::RequestAsyncLoad(m_SpellNiagara.ToSoftObjectPath());
}

void UBaseSpell::SetDecoratedSelf(ISpell* decoratedSelf)
{
	if (decoratedSelf == nullptr)
	{
		LOG_ERROR("Tried to set Spell DecoratedSelf with invalid spell");
		return;
	}

	m_DecoratedSelf = decoratedSelf->_getUObject();
}

void UBaseSpell::LoadHitNiagara()
{

}

bool UBaseSpell::CanCast()
{
	if (m_CooldownTimer > 0.0f)
	{
		return false;
	}

	return true;
}

bool UBaseSpell::CastSpell()
{
	if (m_CooldownTimer > 0.0f)
	{
		LOG_WARNING("Check CanCast before calling CastSpell");
		LOG_ONSCREEN(-1, 5.0f, FColor::Yellow, "Check CanCast before calling CastSpell");
		return false;
	}
	
	m_CooldownTimer = m_Cooldown;
	return true;
}

void UBaseSpell::Update(float deltaTime)
{
	if (m_Cooldown < 0)
	{
		return;
	}

	m_CooldownTimer -= deltaTime;
}

void UBaseSpell::OnOverlap(AActor* overlapActor, FVector spellOverlapLocation, int damage)
{		
	if (m_DecoratedSelf->DealDamage(overlapActor, damage) == false)
	{
		m_DecoratedSelf->ApplyEffects(overlapActor);
	}
}

void UBaseSpell::OnHit(AActor* hitActor, FVector spellHitLocation)
{
	if (hitActor != nullptr && m_HitActors.Contains(hitActor) == false)
	{
		m_HitActors.Add(hitActor);
	}
}

void UBaseSpell::ProcessHit(FVector spellHitLocation)
{
	for (auto actor : m_HitActors)
	{
		int damage = 0;
		m_DecoratedSelf->ProcessHitDamage(damage, actor->GetActorLocation(), spellHitLocation);

		if (m_DecoratedSelf->DealDamage(actor, damage) == false)
		{
			m_DecoratedSelf->ApplyEffects(actor);
		}
	}

	m_DecoratedSelf->SpawnHitNiagara(spellHitLocation);

	m_HitActors.Empty();
}

void UBaseSpell::SpawnHitNiagara(FVector spellHitLocation)
{
	if (m_HitNiagara.IsValid() == false)
	{
		return;
	}

	if (UWorld* worldContext = UCoreFunctionLibrary::GetGameWorld())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(worldContext, m_HitNiagara.Get(), spellHitLocation);
	}
}

void UBaseSpell::Fire(FVector direction)
{
}

void UBaseSpell::ApplyEffects(AActor* hitActor)
{
}

ISpellCaster* UBaseSpell::GetSpellOwner()
{
	if (m_SpellOwner == nullptr)
	{
		LOG_ERROR("Spell has invalid SpellOwner");
		return nullptr;
	}

	return m_SpellOwner.GetInterface();
}

TArray<TObjectPtr<AActor>> UBaseSpell::GetHitActors()
{
	return m_HitActors;
}
