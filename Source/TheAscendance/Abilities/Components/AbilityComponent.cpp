// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Characters/BaseCharacter.h"
#include "TheAscendance/Abilities/Interfaces/Ability.h"
#include "TheAscendance/Abilities/Structs/AbilityData.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Abilities/AbilityFactory.h"

// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	m_Abilities.SetNum(UAbilityHelpers::MaxAbilities);
}

void UAbilityComponent::SetAbilities(const TArray<FGameplayTag>& abilityTags)
{
	if (m_Owner == nullptr)
	{
		LOG_ERROR("[ABILITY COMPONENT] SetAbilities was called but Owner is invalid");
		return;
	}

	check(m_Abilities.Num() == UAbilityHelpers::MaxAbilities);
	//check(abilityTags.Num() == MaxAbilities);

	int maxCount = FMath::Min(abilityTags.Num(), UAbilityHelpers::MaxAbilities);

	for (int i = 0; i < maxCount; i++)
	{
		if (abilityTags[i].IsValid() == false)
		{
			if (m_Abilities[i] != nullptr)
			{
				m_Abilities[i] = nullptr;
			}

			continue;
		}

		if (m_Abilities.IsValidIndex(i) == false)
		{
			LOG_ERROR("[ABILITY COMPONENT] SetSpells iterator went out of valid range");
			return;
		}

		if (IAbility* ability = m_Abilities[i].GetInterface())
		{
			if (ability->GetAbilityTag() == abilityTags[i])
			{
				//No Change
				continue;
			}
		}

		//Below failure checks are already logged internally
		if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
		{
			if (IAbility* ability = gameMode->CreateAbilityFromTag(abilityTags[i], this))
			{
				m_Abilities[i] = ability->_getUObject();
			}
		}
	}

	TriggerOnAbilitiesUpdate();
}

void UAbilityComponent::TestSetAbilities(const TArray<FGameplayTag>& abilityTags)
{
	if (m_Owner == nullptr)
	{
		LOG_ERROR("[ABILITY COMPONENT] SetAbilities was called but Owner is invalid");
		return;
	}

	check(m_Abilities.Num() == UAbilityHelpers::MaxAbilities);
	check(abilityTags.Num() == UAbilityHelpers::MaxAbilities);

	int maxCount = FMath::Min(abilityTags.Num(), UAbilityHelpers::MaxAbilities);

	for (int i = 0; i < maxCount; i++)
	{
		if (abilityTags[i].IsValid() == false)
		{
			if (m_Abilities[i] != nullptr)
			{
				m_Abilities[i] = nullptr;
			}

			continue;
		}

		if (m_Abilities.IsValidIndex(i) == false)
		{
			LOG_ERROR("[ABILITY COMPONENT] SetSpells iterator went out of valid range");
			return;
		}

		if (IAbility* ability = m_Abilities[i].GetInterface())
		{
			if (ability->GetAbilityTag() == abilityTags[i])
			{
				//No Change
				continue;
			}
		}

		//Below failure checks are already logged internally
		if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
		{
			if (IAbility* ability = gameMode->CreateAbilityFromTag(abilityTags[i], this))
			{
				m_Abilities[i] = ability->_getUObject();
			}
		}
	}
}

void UAbilityComponent::StartAbility(UAbilityHelpers::EAbilitySlot slot)
{
	//for(const auto& ability : m_Abilities)
	//{
	//	if (ability == nullptr)
	//	{
	//		continue;
	//	}

	//	if (ability->CanStart())
	//	{
	//		return;
	//	}
	//}

	if (m_Abilities[slot - 1] == nullptr)
	{
		LOG_WARNING("[ABILITY COMPONENT] Tried to StartAbility, but the ability slot was empty (not an error if this is expected)");
		return;
	}

	if (m_Abilities[slot - 1]->CanStart() == false)
	{
		return;
	}

	m_ActiveAbility = m_Abilities[slot - 1];
	m_ActiveAbility->Start();
}

void UAbilityComponent::TriggerAbility()
{
	if (m_ActiveAbility != nullptr)
	{
		m_ActiveAbility->TriggerAbility();
	}
}

void UAbilityComponent::StopAbility()
{
	if (m_ActiveAbility != nullptr)
	{
		m_ActiveAbility->Stop();
	}
}

void UAbilityComponent::OnInputReleased()
{
	if (m_ActiveAbility != nullptr)
	{
		m_ActiveAbility->OnInputReleased();
	}
}

// Called when the game starts
void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ABaseCharacter* owner = Cast<ABaseCharacter>(GetOwner()))
	{
		m_Owner = owner;
	}
}

float UAbilityComponent::PlayAnimMontageOnOwner(UAnimMontage* animation)
{
	if(m_Owner == nullptr)
	{
		LOG_ERROR("[ABILITY COMPONENT] PlayAnimation was called but Owner is invalid");
		return 0.0f;
	}

	return m_Owner->PlayAnimationMontage(animation);
}

void UAbilityComponent::TriggerOnAbilitiesUpdate()
{
	TArray<FAbilityInfo> abilityInfo;

	for(const auto& ability : m_Abilities)
	{
		if (ability == nullptr)
		{
			abilityInfo.Add(FAbilityInfo());
			continue;
		}

		if(UAbilityData* data = ability->GetAbilityData())
		{
			FAbilityInfo info;
			info.Icon = data->AbilityIcon;
			abilityInfo.Add(info);
		}
	}

	OnAbilitiesUpdate.Broadcast(abilityInfo);
}

FVector UAbilityComponent::GetCastLocation()
{
	if (m_Owner == nullptr)
	{
		LOG_ERROR("[ABILITY COMPONENT] GetCastLocation was called but Owner is invalid");
		return FVector::ForwardVector;
	}

	return m_Owner->GetActorLocation();//m_Owner->GetCastStartLocation();
}

FVector UAbilityComponent::GetCastForward()
{
	if (m_Owner == nullptr)
	{
		LOG_ERROR("[ABILITY COMPONENT] GetCastLocation was called but Owner is invalid");
		return FVector::ForwardVector;
	}

	return m_Owner->GetActorForwardVector();//m_Owner->GetCastStartForward();
}

void UAbilityComponent::AffectOwnerStat(ECharacterStat stat, int amount)
{
	if(m_Owner == nullptr)
	{
		LOG_ERROR("[ABILITY COMPONENT] AffectOwnerStat was called but Owner is invalid");
		return;
	}

	m_Owner->AdjustStat(stat, -amount);
}

float UAbilityComponent::GetOwnerStat(ECharacterStat stat)
{
	if (m_Owner == nullptr)
	{
		LOG_ERROR("[ABILITY COMPONENT] AffectOwnerStat was called but Owner is invalid");
		return -1.0f;
	}

	return m_Owner->GetStat(stat);
}

// Called every frame
void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(m_Abilities.Num() == 0)
	{
		return;
	}

	for(const auto& ability : m_Abilities)
	{
		if (ability == nullptr)
		{
			continue;
		}

		ability->Update(DeltaTime);
	}
}