// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadoutComponent.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/GameplayTagHelpers.h"
#include "TheAscendance/Characters/BaseCharacter.h"

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
	FGameplayTag equipmentTag = FGameplayTag::RequestGameplayTag(TEXT("Item.Equipment"));

	if (itemTag.MatchesTag(equipmentTag) == false)
	{
		LOG_ERROR("[LOADOUT COMPONENT] Tried to equip non-equipment Item");
		return;
	}

	if (m_Owner.IsValid() == false)
	{
		m_Owner = Cast<ABaseCharacter>(GetOwner());

		if(m_Owner.IsValid() == false)
		{
			LOG_ERROR("[LOADOUT COMPONENT] LoadoutComponent has invalid owner");
			return;
		}
	}

	UnEquipItem(part);

	if(m_Owner->EquipItem(part, itemTag) == false)
	{
		LOG_ERROR("[LOADOUT COMPONENT] %s failed to equip item %s", *m_Owner->GetName(), *itemTag.ToString());
		return;
	}

	m_Loadout.Add(MakeShared<FLoadoutSlotData>(itemTag, part));
	LOG_INFO("[LOADOUT COMPONENT] %s equipped item %s", *m_Owner->GetName(), *itemTag.ToString());

	if (OnEquipmentUpdated.IsBound())
	{
		FEquipmentMap newMap;

		for (const auto& data : m_Loadout)
		{
			if (data.IsValid() == false)
			{
				continue;
			}

			newMap.Map.Add(data->EquippedPart, data->ItemTag);
		}

		OnEquipmentUpdated.Broadcast(newMap);
	}
}

void ULoadoutComponent::UnEquipItem(EEquippablePart part)
{
	bool loadoutChanged = false;

	for (const auto data : m_Loadout)
	{
		if (data->EquippedPart == part)
		{
			LOG_INFO("[LOADOUT COMPONENT] %s unequipped item %s", *m_Owner->GetName(), *data->ItemTag.ToString());
			m_Owner->UnEquipItem(part);
			m_Loadout.Remove(data);
			loadoutChanged = true;
			break;
		}
	}

	if (loadoutChanged == true && OnEquipmentUpdated.IsBound())
	{
		FEquipmentMap newMap;

		for (const auto& data : m_Loadout)
		{
			if (data.IsValid() == false)
			{
				continue;
			}

			newMap.Map.Add(data->EquippedPart, data->ItemTag);
		}

		OnEquipmentUpdated.Broadcast(newMap);
	}
}

void ULoadoutComponent::SetSpells(const TArray<FGameplayTag>& spellTags)
{
	if (m_SpellTags == spellTags)
	{
		LOG_INFO("[LOADOUT COMPONENT] SetSpells made no changes")
		return;
	}

	m_SpellTags = spellTags;

	if (OnSpellsUpdated.IsBound())
	{
		OnSpellsUpdated.Broadcast(spellTags);
	}
}

const TArray<FGameplayTag>& ULoadoutComponent::GetSpellTags() const
{
	return m_SpellTags;
}

bool ULoadoutComponent::Contains(EEquippablePart part)
{
	for (const auto data : m_Loadout)
	{
		if (data->EquippedPart == part)
		{
			return true;
		}
	}

	return false;
}

// Called when the game starts
void ULoadoutComponent::BeginPlay()
{
	Super::BeginPlay();
}
