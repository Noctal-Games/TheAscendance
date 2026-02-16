// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadoutComponent.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Characters/BaseCharacter.h"

// Sets default values for this component's properties
ULoadoutComponent::ULoadoutComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void ULoadoutComponent::EquipItem(EEquippablePart part, int itemID)
{
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

	if(m_Owner->EquipItem(part, itemID) == false)
	{
		LOG_ERROR("[LOADOUT COMPONENT] %s failed to equip item %i", *m_Owner->GetName(), itemID);
		return;
	}

	m_Loadout.Add(MakeShared<FLoadoutSlotData>(itemID, part));
	LOG_INFO("[LOADOUT COMPONENT] %s equipped item %i", *m_Owner->GetName(), itemID);
}

void ULoadoutComponent::UnEquipItem(EEquippablePart part)
{
	for (const auto data : m_Loadout)
	{
		if (data->EquippedPart == part)
		{
			LOG_INFO("[LOADOUT COMPONENT] %s unequipped item %i", *m_Owner->GetName(), data->ItemID);
			data->ItemID = 0;
			m_Owner->UnEquipItem(part);
			return;
		}
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
