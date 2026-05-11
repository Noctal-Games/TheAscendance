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

	m_SpellTags.SetNum(UAbilityHelpers::MaxAbilities);
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

void ULoadoutComponent::SetSpells(const TArray<FGameplayTag>& spellTags)
{
	if(spellTags.Num() != UAbilityHelpers::MaxAbilities)
	{
		LOG_ERROR("[LOADOUT COMPONENT] SetSpells was given an array of size %d but expected size is %d", spellTags.Num(), UAbilityHelpers::MaxAbilities)
		return;
	}

	if (m_SpellTags == spellTags)
	{
		LOG_INFO("[LOADOUT COMPONENT] SetSpells made no changes")
		return;
	}

	m_SpellTags = spellTags;

	for(int i = 0; i < spellTags.Num(); i++)
	{
		const UEnum* abilityEnum = StaticEnum<EAbilitySlot>();

		if (abilityEnum->IsValidEnumValue(i))
		{
			EAbilitySlot slot = static_cast<EAbilitySlot>(i);
			m_TestSpellTags.Add(slot, spellTags[i]);
		}
		else
		{
			LOG_WARNING("Invalid enum value: %d", i);
		}
	}

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

const TArray<FGameplayTag>& ULoadoutComponent::GetSpellTags() const
{
	return m_SpellTags;
}

TMap<EAbilitySlot, FGameplayTag> ULoadoutComponent::GetSpellsCopy() const
{
	return m_TestSpellTags;
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