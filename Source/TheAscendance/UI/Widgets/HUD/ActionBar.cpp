// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionBar.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Abilities/Components/AbilityComponent.h"
#include "TheAscendance/Abilities/Structs/AbilityInfo.h"
#include "ActionBarIcon.h"
#include "TheAscendance/Abilities/Components/AbilityComponent.h"

void UActionBar::Init(UAbilityComponent* abilityComponent)
{
	if (abilityComponent == nullptr)
	{
		LOG_ERROR("[ACTION BAR ICON] Ability Component is invalid.");
		return;
	}

	abilityComponent->OnAbilitiesUpdate.AddUObject(this, &UActionBar::OnAbilitiesUpdated);
}

void UActionBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UActionBar::NativeDestruct()
{
	Super::NativeDestruct();
}

void UActionBar::OnAbilitiesUpdated(const TArray<FAbilityInfo>& abilityInfo)
{
	if(abilityInfo.Num() != UAbilityHelpers::MaxAbilities)
	{
		LOG_ERROR("[ACTION BAR] Ability Icon List should be of length %i", UAbilityHelpers::MaxAbilities);
		return;
	}

	abilityInfo.IsValidIndex(0) ? m_MainHandActionSlot1->LoadAbilityIcon(abilityInfo[0].Icon) : m_MainHandActionSlot1->ClearAbilityIcon();
	abilityInfo.IsValidIndex(1) ? m_MainHandActionSlot2->LoadAbilityIcon(abilityInfo[1].Icon) : m_MainHandActionSlot2->ClearAbilityIcon();
	abilityInfo.IsValidIndex(2) ? m_OffHandActionSlot1->LoadAbilityIcon(abilityInfo[2].Icon) : m_OffHandActionSlot1->ClearAbilityIcon();
	abilityInfo.IsValidIndex(3) ? m_OffHandActionSlot2->LoadAbilityIcon(abilityInfo[3].Icon) : m_OffHandActionSlot2->ClearAbilityIcon();
}
