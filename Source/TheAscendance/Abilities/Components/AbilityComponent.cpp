// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/AbilityHelpers.h"
#include "TheAscendance/Characters/BaseCharacter.h"
#include "TheAscendance/Abilities/Interfaces/Ability.h"
#include "TheAscendance/Abilities/Structs/AbilityData.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Abilities/AbilityFactory.h"
#include "TheAscendance/Abilities/BaseAbility.h"

// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	m_Abilities.SetNum(UAbilityHelpers::MaxAbilities);
}

void UAbilityComponent::SetAbilities(const TMap<EAbilitySlot, FGameplayTag>& abilityTags)
{
	if (m_Owner == nullptr)
	{
		LOG_ERROR("[ABILITY COMPONENT] SetAbilities was called but Owner is invalid");
		return;
	}

	APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode();

	if (gameMode == nullptr)
	{
		LOG_ERROR("[ABILITY COMPONENT] SetAbilities was called but GameMode is invalid");
		return;
	}

	m_Abilities.Empty();

	TMap<EAbilitySlot, UAbilityData*> abilityData;

	for (const auto& abilityTagPair : abilityTags)
	{
		if (abilityTagPair.Value.IsValid() == false || abilityTagPair.Value == FGameplayTag::EmptyTag)
		{
			continue;
		}

		if (UAbilityData* data = gameMode->GetAbilityData(abilityTagPair.Value))
		{
			abilityData.Add(abilityTagPair.Key, data);
		}
	}

	ProcessAbilityPair(abilityData, EAbilitySlot::MAINHAND_PRIMARY, EAbilitySlot::OFFHAND_PRIMARY, FString("PRIMARY"), gameMode);
	ProcessAbilityPair(abilityData, EAbilitySlot::MAINHAND_ALT, EAbilitySlot::OFFHAND_ALT, FString("ALT"), gameMode);

	TriggerOnAbilitiesUpdate();
}

void UAbilityComponent::StartAbility(EAbilitySlot slot)
{
	for(const auto& ability : m_Abilities)
	{
		if (ability == nullptr)
		{
			continue;
		}

		const FAbilityInfo& info = ability->GetAbilityInfo();

		if (info.SlotsUsed.Contains(slot) == false)
		{
			continue;
		}

		if (ability->CanStart() == false)
		{
			return;
		}

		m_ActiveAbility = ability;
		m_ActiveAbility->Start();
		return;
	}
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

		abilityInfo.Add(ability->GetAbilityInfo());
	}

	OnAbilitiesUpdate.Broadcast(abilityInfo);
}

void UAbilityComponent::ProcessAbilityPair(TMap<EAbilitySlot, UAbilityData*>& abilityData,EAbilitySlot mainSlot, EAbilitySlot offSlot, const FString& label, APlayableGameMode* gameMode)
{
	UAbilityData* mainData = nullptr;
	UAbilityData* offData = nullptr;

	if (UAbilityData** found = abilityData.Find(mainSlot))
	{
		mainData = *found;
	}

	if (UAbilityData** found = abilityData.Find(offSlot))
	{
		offData = *found;
	}

	if (mainData != nullptr && offData != nullptr)
	{
		if (mainData->IsTwoHanded && offData->IsTwoHanded)
		{
			LOG_WARNING("[ABILITY COMPONENT] Both %s abilities are two-handed. Prioritizing MAINHAND.", *label);
			offData = nullptr;
		}
		else if (mainData->IsTwoHanded)
		{
			LOG_WARNING("[ABILITY COMPONENT] MAINHAND_%s is two-handed. Ignoring OFFHAND ability.", *label);
			offData = nullptr;
		}
		else if (offData->IsTwoHanded)
		{
			LOG_WARNING("[ABILITY COMPONENT] OFFHAND_%s is two-handed. Ignoring MAINHAND ability.", *label);
			mainData = nullptr;
		}
	}

	if (mainData != nullptr)
	{
		if (IAbility* ability = gameMode->CreateAbilityFromData(mainData, this))
		{
			ability->AddSlot(mainSlot);

			if (mainData->IsTwoHanded)
			{
				ability->AddSlot(offSlot);
			}

			m_Abilities.Add(ability->_getUObject());
		}
	}

	if (offData != nullptr)
	{
		if (IAbility* ability = gameMode->CreateAbilityFromData(offData, this))
		{
			ability->AddSlot(offSlot);

			if (offData->IsTwoHanded)
			{
				ability->AddSlot(mainSlot);
			}

			m_Abilities.Add(ability->_getUObject());
		}
	}
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

//if (m_Owner == nullptr)
//{
//	LOG_ERROR("[ABILITY COMPONENT] SetAbilities was called but Owner is invalid");
//	return;
//}

//check(m_Abilities.Num() == UAbilityHelpers::MaxAbilities);
////check(abilityTags.Num() == MaxAbilities);

//int maxCount = FMath::Min(abilityTags.Num(), UAbilityHelpers::MaxAbilities);

//for (int i = 0; i < maxCount; i++)
//{
//	if (abilityTags[i].IsValid() == false)
//	{
//		if (m_Abilities[i] != nullptr)
//		{
//			m_Abilities[i] = nullptr;
//		}

//		continue;
//	}

//	if (m_Abilities.IsValidIndex(i) == false)
//	{
//		LOG_ERROR("[ABILITY COMPONENT] SetSpells iterator went out of valid range");
//		return;
//	}

//	if (IAbility* ability = m_Abilities[i].GetInterface())
//	{
//		if (ability->GetAbilityTag() == abilityTags[i])
//		{
//			//No Change
//			continue;
//		}
//	}

//	//Below failure checks are already logged internally
//	if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
//	{
//		if (IAbility* ability = gameMode->CreateAbilityFromTag(abilityTags[i], this))
//		{
//			m_Abilities[i] = ability->_getUObject();
//		}
//	}
//}
//TriggerOnAbilitiesUpdate();


	/*UAbilityData* mainData = nullptr;
	UAbilityData* offData = nullptr;

	if (UAbilityData** found = abilityData.Find(UAbilityHelpers::EAbilitySlot::MAINHAND_PRIMARY))
	{
		mainData = *found;
	}

	if (UAbilityData** found = abilityData.Find(UAbilityHelpers::EAbilitySlot::OFFHAND_PRIMARY))
	{
		offData = *found;
	}

	if (mainData != nullptr && offData != nullptr)
	{
		if (mainData->IsTwoHanded && offData->IsTwoHanded)
		{
			LOG_WARNING("[ABILITY COMPONENT] Both PRIMARY abilities are two-handed. Prioritizing MAINHAND_PRIMARY.");
			offData = nullptr;
		}
		else if (mainData->IsTwoHanded)
		{
			LOG_WARNING("[ABILITY COMPONENT] MAINHAND_PRIMARY is two-handed. Ignoring OFFHAND_PRIMARY ability.");
			offData = nullptr;
		}
		else if (offData->IsTwoHanded)
		{
			LOG_WARNING("[ABILITY COMPONENT] OFFHAND_PRIMARY is two-handed. Ignoring MAINHAND_PRIMARY ability.");
			mainData = nullptr;
		}
	}

	if (mainData != nullptr)
	{
		if (IAbility* ability = gameMode->CreateAbilityFromData(mainData, this))
		{
			m_Abilities.Add(ability->_getUObject());
		}
	}

	if (offData != nullptr)
	{
		if (IAbility* ability = gameMode->CreateAbilityFromData(offData, this))
		{
			m_Abilities.Add(ability->_getUObject());
		}
	}

	mainData = nullptr;
	offData = nullptr;

	if (UAbilityData** found = abilityData.Find(UAbilityHelpers::EAbilitySlot::MAINHAND_ALT))
	{
		mainData = *found;
	}

	if (UAbilityData** found = abilityData.Find(UAbilityHelpers::EAbilitySlot::OFFHAND_ALT))
	{
		offData = *found;
	}

	if (mainData != nullptr && offData != nullptr)
	{
		if (mainData->IsTwoHanded && offData->IsTwoHanded)
		{
			LOG_WARNING("[ABILITY COMPONENT] Both ALT abilities are two-handed. Prioritizing MAINHAND_ALT.");
			offData = nullptr;
		}
		else if (mainData->IsTwoHanded)
		{
			LOG_WARNING("[ABILITY COMPONENT] MAINHAND_ALT is two-handed. Ignoring OFFHAND_ALT ability.");
			offData = nullptr;
		}
		else if (offData->IsTwoHanded)
		{
			LOG_WARNING("[ABILITY COMPONENT] OFFHAND_ALT is two-handed. Ignoring MAINHAND_ALT ability.");
			mainData = nullptr;
		}
	}

	if (mainData != nullptr)
	{
		if (IAbility* ability = gameMode->CreateAbilityFromData(mainData, this))
		{
			m_Abilities.Add(ability->_getUObject());
		}
	}

	if (offData != nullptr)
	{
		if (IAbility* ability = gameMode->CreateAbilityFromData(offData, this))
		{
			m_Abilities.Add(ability->_getUObject());
		}
	}*/

	//check(m_Abilities.Num() == UAbilityHelpers::MaxAbilities);
	//check(abilityTags.Num() == UAbilityHelpers::MaxAbilities);

	//int maxCount = FMath::Min(abilityTags.Num(), UAbilityHelpers::MaxAbilities);

	//for (int i = 0; i < maxCount; i++)
	//{
	//	if (abilityTags[i].IsValid() == false)
	//	{
	//		if (m_Abilities[i] != nullptr)
	//		{
	//			m_Abilities[i] = nullptr;
	//		}

	//		continue;
	//	}

	//	if (m_Abilities.IsValidIndex(i) == false)
	//	{
	//		LOG_ERROR("[ABILITY COMPONENT] SetSpells iterator went out of valid range");
	//		return;
	//	}

	//	if (IAbility* ability = m_Abilities[i].GetInterface())
	//	{
	//		if (ability->GetAbilityTag() == abilityTags[i])
	//		{
	//			//No Change
	//			continue;
	//		}
	//	}

	//	//Below failure checks are already logged internally
	//	if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
	//	{
	//		if (IAbility* ability = gameMode->CreateAbilityFromTag(abilityTags[i], this))
	//		{
	//			m_Abilities[i] = ability->_getUObject();
	//		}
	//	}
	//}


//if (m_Abilities[slot - 1] == nullptr)
//{
//	LOG_WARNING("[ABILITY COMPONENT] Tried to StartAbility, but the ability slot was empty (not an error if this is expected)");
//	return;
//}

//if (m_Abilities[slot - 1]->CanStart() == false)
//{
//	return;
//}

//m_ActiveAbility = m_Abilities[slot - 1];
//m_ActiveAbility->Start();