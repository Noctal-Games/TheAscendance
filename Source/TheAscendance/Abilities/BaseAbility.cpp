// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAbility.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/StreamableFunctionLibrary.h"
#include "Structs/AbilityData.h"
#include "Components/AbilityComponent.h"
#include "TheAscendance/Characters/Interfaces/Susceptible.h"

#include "NiagaraFunctionLibrary.h"

void UBaseAbility::Init(UAbilityComponent* ownerComponent, UAbilityData* abilityData)
{
	m_OwnerComponent = ownerComponent;

	if(m_OwnerComponent == nullptr)
	{
		LOG_ERROR("[BASE ABILITY] Failed to Init Ability, OwnerComponent was invalid");
		return;
	}

	m_AbilityData = abilityData;

	if (m_AbilityData == nullptr)
	{
		LOG_ERROR("[BASE ABILITY] Failed to Init Ability, AbilityData was invalid");
		return;
	}

	m_AbilityAnimation = m_AbilityData->AbilityAnimation;

	if(m_AbilityAnimation.IsNull() == true)
	{
		LOG_ERROR("[BASE ABILITY] Ability - %s: AbilityAnimation was invalid", *GetAbilityTag().ToString());
		return;
	}

	m_AbilityInfo.Tag = m_AbilityData->AbilityTag;
	m_AbilityInfo.Icon = m_AbilityData->AbilityIcon;
	m_Cooldown = m_AbilityData->Cooldown;

	UStreamableFunctionLibrary::RequestAsyncLoad(m_AbilityAnimation.ToSoftObjectPath());

	if (m_DecoratedSelf != nullptr)
	{
		m_DecoratedSelf->LoadHitNiagara();
	}
}

void UBaseAbility::SetDecoratedSelf(IAbility* decoratedSelf)
{	
	if (decoratedSelf == nullptr)
	{
		LOG_ERROR("[BASE ABILITY] Tried to set Ability DecoratedSelf with invalid ability");
		return;
	}

	m_DecoratedSelf = decoratedSelf->_getUObject();
}

void UBaseAbility::Start()
{
	LOG_ONSCREEN(-1, 5.0f, FColor::Yellow, "Ability - %s: STARTED", *GetAbilityTag().ToString());
	Execute();
}

void UBaseAbility::Stop()
{
	LOG_ONSCREEN(-1, 5.0f, FColor::Yellow, "Ability - %s: STOP", *GetAbilityTag().ToString());

	if (m_AbilityDurationHandle.IsValid())
	{
		UCoreFunctionLibrary::ClearTimerHandle(m_AbilityDurationHandle, FString("Stop Ability"));
	}
}

void UBaseAbility::Execute()
{
	LOG_ONSCREEN(-1, 5.0f, FColor::Yellow, "Ability - %s: EXECUTE", *GetAbilityTag().ToString());

	if (m_AbilityAnimation.IsNull() == true)
	{
		LOG_ERROR("[BASE ABILITY] Start was called but AbilityAnimation is invalid");
		return;
	}

	//Timer to prevent animation locking up attacks. If an animation fails to notify, the ability and character attack state will be reset by default. 
	float duration = PlayAnimMontageOnOwner(m_AbilityAnimation.Get());
	UCoreFunctionLibrary::SetTimer(m_AbilityDurationHandle, this, &UBaseAbility::Stop, duration);
}

void UBaseAbility::TriggerAbility()
{
	LOG_ONSCREEN(-1, 5.0f, FColor::Yellow, "Ability - %s: TRIGGER", *GetAbilityTag().ToString());

	if (m_CooldownTimer > 0.0f)
	{
		LOG_WARNING("[BASE ABILITY] Check CanStart before activating ability");
		return;
	}
	
	TriggerCooldown();
	AffectOwnerStat();
}

void UBaseAbility::OnInputReleased()
{
	LOG_ONSCREEN(-1, 5.0f, FColor::Yellow, "Ability - %s: RELEASED", *GetAbilityTag().ToString());
}

void UBaseAbility::OnOverlap(AActor* overlapActor, const FVector& overlapLocation, int damage)
{
	if (m_DecoratedSelf->DealDamage(overlapActor, damage) == false)
	{
		m_DecoratedSelf->ApplyEffects(overlapActor);
	}
}

void UBaseAbility::OnHit(AActor* hitActor, const FVector& hitLocation)
{
	if (hitActor != nullptr && m_HitActors.Contains(hitActor) == false)
	{
		m_HitActors.Add(hitActor);
	}
}

void UBaseAbility::ProcessHit(const FVector& hitLocation)
{
	for (auto actor : m_HitActors)
	{
		int damage = 0;
		m_DecoratedSelf->ProcessHitDamage(damage, actor->GetActorLocation(), hitLocation);

		if (m_DecoratedSelf->DealDamage(actor, damage) == false)
		{
			m_DecoratedSelf->ApplyEffects(actor);
		}
	}

	m_DecoratedSelf->SpawnHitNiagara(hitLocation);
	m_HitActors.Empty();
}

bool UBaseAbility::DealDamage(AActor* hitActor, int damage)
{
	if (ISusceptible* target = Cast<ISusceptible>(hitActor))
	{
		target->Damage(damage, true);
		return target->IsDead();
	}

	return true;
}

void UBaseAbility::SpawnHitNiagara(const FVector& hitLocation)
{
	if (m_HitNiagara.IsValid() == false)
	{
		return;
	}

	if (UWorld* worldContext = UCoreFunctionLibrary::GetGameWorld())
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(worldContext, m_HitNiagara.Get(), hitLocation);
	}
}

void UBaseAbility::LoadHitNiagara()
{ 
	if(m_AbilityData == nullptr)
	{
		LOG_ERROR("[BASE ABILITY] Tried to Load HitNiagara with invalid AbilityData");
		return;
	}

	m_HitNiagara = m_AbilityData->HitNiagara;

	if (m_HitNiagara.IsNull() == true)
	{
		LOG_WARNING("[BASE ABILITY] Tried to Load HitNiagara with invalid HitNiagara");
		return;
	}

	UStreamableFunctionLibrary::RequestAsyncLoad(m_HitNiagara.ToSoftObjectPath());
}

const FGameplayTag& UBaseAbility::GetAbilityTag() const
{
	if (m_AbilityData == nullptr)
	{
		LOG_ERROR("[BASE ABILITY] GetAbilityTag was called but AbilityData is invalid");
		return FGameplayTag::EmptyTag;
	}

	return m_AbilityData->AbilityTag;
}

TArray<TObjectPtr<AActor>> UBaseAbility::GetHitActors()
{
	return m_HitActors;
}

void UBaseAbility::ProcessOverlapDamage(int& damage)
{
}

void UBaseAbility::ProcessHitDamage(int& damage, const FVector& targetLocation, const FVector& hitLocation)
{
}

void UBaseAbility::ApplyEffects(AActor* hitActor)
{
}

float UBaseAbility::PlayAnimMontageOnOwner(UAnimMontage* animation)
{
	if (m_OwnerComponent.IsValid() == false)
	{
		LOG_ERROR("[BASE ABILITY] PlayAnimMontageOnOwner was called but OwnerComponent is invalid");
		return 0.0f;
	}

	return m_OwnerComponent->PlayAnimMontageOnOwner(animation);
}

UAbilityData* UBaseAbility::GetAbilityData()
{
	return m_AbilityData;
}

AActor* UBaseAbility::GetAbilityOwner()
{
	if (m_OwnerComponent.IsValid() == false)
	{
		LOG_ERROR("[BASE ABILITY] GetOwner was called but OwnerComponent is invalid");
		return nullptr;
	}

	return m_OwnerComponent->GetOwner();
}

bool UBaseAbility::CanStart() const
{
	if (m_OwnerComponent == nullptr)
	{
		LOG_ERROR("[BASE ABILITY] CanStart was called but OwnerComponent is invalid");
		return false;
	}

	if (m_AbilityData == nullptr)
	{
		LOG_ERROR("[BASE ABILITY] CanStart was called but AbilityData is invalid");
		return false;
	}

	if (m_CooldownTimer > 0.0f)
	{
		return false;
	}

	float stat = m_OwnerComponent->GetOwnerStat(m_AbilityData->UsedStat);
	return stat >= m_AbilityData->StatCost;
}

void UBaseAbility::Update(float deltaTime)
{   
	if (m_Cooldown < 0)
	{
		return;
	}

	m_CooldownTimer -= deltaTime;
}

const FAbilityInfo& UBaseAbility::GetAbilityInfo() const
{
	return m_AbilityInfo;
}

void UBaseAbility::AddSlot(EAbilitySlot slot)
{
	m_AbilityInfo.SlotsUsed.Add(slot);
}

void UBaseAbility::AffectOwnerStat()
{
	if (m_OwnerComponent == nullptr)
	{
		LOG_ERROR("[BASE ABILITY] AffectOwnerStat was called but OwnerComponent is invalid");
		return;
	}

	if (m_AbilityData == nullptr)
	{
		LOG_ERROR("[BASE ABILITY] AffectOwnerStat was called but AbilityData is invalid");
		return;
	}

	m_OwnerComponent->AffectOwnerStat(m_AbilityData->UsedStat, m_AbilityData->StatCost);
}

void UBaseAbility::TriggerCooldown()
{
	m_CooldownTimer = m_Cooldown;

	if (m_OwnerComponent != nullptr && m_AbilityData != nullptr)
	{
		m_OwnerComponent->OnAbilityCooldown.Broadcast(m_AbilityData->AbilityTag, m_CooldownTimer, m_Cooldown);
	}
}
