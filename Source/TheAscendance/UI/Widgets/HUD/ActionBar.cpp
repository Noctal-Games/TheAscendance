// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionBar.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Abilities/Components/AbilityComponent.h"
#include "TheAscendance/Abilities/Structs/AbilityInfo.h"
#include "ActionBarIcon.h"
#include "TheAscendance/Abilities/Components/AbilityComponent.h"
#include "TheAscendance/Abilities/Enums/AbilitySlot.h"

void UActionBar::Init(UAbilityComponent* abilityComponent)
{
	if (abilityComponent == nullptr)
	{
		LOG_ERROR("[ACTION BAR ICON] Ability Component is invalid.");
		return;
	}

	abilityComponent->OnAbilitiesUpdate.AddUObject(this, &UActionBar::OnAbilitiesUpdated);
	abilityComponent->OnAbilityCooldown.AddUObject(this, &UActionBar::OnCooldownTriggered);
}

void UActionBar::OnCooldownTriggered(const FGameplayTag& abilityTag, float remaining, float max)
{
	m_MainHandActionSlot1->OnCooldownTriggered(abilityTag, remaining, max);
	m_MainHandActionSlot2->OnCooldownTriggered(abilityTag, remaining, max);
	m_OffHandActionSlot1->OnCooldownTriggered(abilityTag, remaining, max);
	m_OffHandActionSlot2->OnCooldownTriggered(abilityTag, remaining, max);
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
	TMap<EAbilitySlot, FAbilityInfo> slotToAbility;

	for (const auto& info : abilityInfo)
	{
		for (EAbilitySlot slot : info.SlotsUsed)
		{
			slotToAbility.FindOrAdd(slot) = info;
		}
	}

	auto applySlot = [&](EAbilitySlot slot, UActionBarIcon* widget)
		{
			const FAbilityInfo* info = slotToAbility.Find(slot);

			if (!info || info->Icon.IsNull())
			{
				widget->ClearAbilityIcon();
				return;
			}

			widget->LoadAbility(info->Tag, info->Icon);
		};

	applySlot(EAbilitySlot::MAINHAND_PRIMARY, m_MainHandActionSlot1);
	applySlot(EAbilitySlot::MAINHAND_ALT, m_MainHandActionSlot2);
	applySlot(EAbilitySlot::OFFHAND_PRIMARY, m_OffHandActionSlot1);
	applySlot(EAbilitySlot::OFFHAND_ALT, m_OffHandActionSlot2);
}


//for(const auto& info : abilityInfo)
//{
//	for (const auto& slot : info.SlotsUsed)
//	{
//		switch (slot)
//		{
//			case EAbilitySlot::MAINHAND_PRIMARY:
//				info.Icon.IsNull() ? m_MainHandActionSlot1->ClearAbilityIcon() : m_MainHandActionSlot1->LoadAbilityIcon(info.Icon);
//				break;
//			case EAbilitySlot::MAINHAND_ALT:
//				info.Icon.IsNull() ? m_MainHandActionSlot1->ClearAbilityIcon() : m_MainHandActionSlot2->LoadAbilityIcon(info.Icon);
//				break;
//			case EAbilitySlot::OFFHAND_PRIMARY:
//				info.Icon.IsNull() ? m_OffHandActionSlot1->ClearAbilityIcon() : m_OffHandActionSlot1->LoadAbilityIcon(info.Icon);
//				break;
//			case EAbilitySlot::OFFHAND_ALT:
//				info.Icon.IsNull() ? m_OffHandActionSlot2->ClearAbilityIcon() : m_OffHandActionSlot2->LoadAbilityIcon(info.Icon);
//				break;
//			default:
//				LOG_ERROR("[ACTION BAR] Invalid ability slot used in ability info.");
//				break;
//		}
//	}
//}

//abilityInfo.IsValidIndex(0) ? m_MainHandActionSlot1->LoadAbilityIcon(abilityInfo[0].Icon) : m_MainHandActionSlot1->ClearAbilityIcon();
//abilityInfo.IsValidIndex(1) ? m_MainHandActionSlot2->LoadAbilityIcon(abilityInfo[1].Icon) : m_MainHandActionSlot2->ClearAbilityIcon();
//abilityInfo.IsValidIndex(2) ? m_OffHandActionSlot1->LoadAbilityIcon(abilityInfo[2].Icon) : m_OffHandActionSlot1->ClearAbilityIcon();
//abilityInfo.IsValidIndex(3) ? m_OffHandActionSlot2->LoadAbilityIcon(abilityInfo[3].Icon) : m_OffHandActionSlot2->ClearAbilityIcon();