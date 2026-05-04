// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadoutComponent.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/AbilityHelpers.h"
#include "TheAscendance/Core/GameplayTagHelpers.h"
#include "TheAscendance/Characters/BaseCharacter.h"
#include "TheAscendance/Abilities/Components/AbilityComponent.h"

// Sets default values for this component's properties
ULoadoutComponent::ULoadoutComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}

void ULoadoutComponent::EquipItem(EEquippablePart part, const FGameplayTag& itemTag)
{
	if (Contains(part))
	{
		for (auto& data : m_Loadout)
		{
			if (data.EquippedPart == part)
			{
				data.ItemTag = itemTag;
				break;
			}
		}
	}
	else
	{
		m_Loadout.Add(FLoadoutSlotData(itemTag, part));
	}
}

void ULoadoutComponent::BlockEquipItem(EEquippablePart part)
{
	FGameplayTag blockedTag = FGameplayTag::RequestGameplayTag(TEXT("Equipment.Slot.Blocked"));

	if (Contains(part))
	{
		for (auto& data : m_Loadout)
		{
			if (data.EquippedPart == part)
			{
				//EmptyTag used to block equipping an item in this slot, instead of using an invalid tag, to avoid confusion with unequipped slots (used for two-handed items, etc)
				data.ItemTag = blockedTag;
				break;
			}
		}
	}
	else
	{
		m_Loadout.Add(FLoadoutSlotData(blockedTag, part));
	}
}

void ULoadoutComponent::UnEquipItem(EEquippablePart part)
{
	for (auto& data : m_Loadout)
	{
		if (data.EquippedPart == part)
		{
			data.ItemTag = FGameplayTag();
			break;
		}
	}
}

void ULoadoutComponent::SetSpells(const TMap<EAbilitySlot, FGameplayTag>& spellTags)
{
	m_SpellTags = spellTags;

	if (OnSpellsUpdate.IsBound())
	{
		OnSpellsUpdate.Broadcast();
	}
}

bool ULoadoutComponent::IsPartEquipped(const EEquippablePart& part)
{
	for (const auto data : m_Loadout)
	{
		if (data.EquippedPart != part)
		{
			continue;
		}

		bool isValid = data.ItemTag.IsValid();
		return isValid;
	}

	LOG_WARNING("[LOADOUT COMPONENT] IsPartEquipped was called for part %s but it was not found in the loadout", *UEnum::GetValueAsString(part));
	return false;
}

const TMap<EAbilitySlot, FGameplayTag>& ULoadoutComponent::GetSpellTags() const
{
	return m_SpellTags;
}

TMap<EAbilitySlot, FGameplayTag> ULoadoutComponent::GetSpellsCopy() const
{
	return m_SpellTags;
}

bool ULoadoutComponent::Contains(EEquippablePart part)
{
	for (const auto data : m_Loadout)
	{
		if (data.EquippedPart == part)
		{
			return true;
		}
	}

	return false;
}