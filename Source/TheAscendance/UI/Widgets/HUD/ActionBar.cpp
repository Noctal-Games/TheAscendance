// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionBar.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Characters/Components/LoadoutComponent.h"
#include "ActionBarIcon.h"

void UActionBar::Init(ULoadoutComponent* ownerLoadoutComponent)
{
	m_OwnerLoadoutComponent = ownerLoadoutComponent;

	if (m_OwnerLoadoutComponent.IsValid() == false)
	{
		LOG_ERROR("[ACTION BAR] Tried to Init but OwnerLoadoutComponent was invalid");
		return;
	}

	m_OnSpellsUpdatedHandled = m_OwnerLoadoutComponent->OnSpellsUpdated.AddUObject(this, &UActionBar::OnSpellsUpdated);
	m_OnEquipmentUpdatedHandled = m_OwnerLoadoutComponent->OnEquipmentUpdated.AddUObject(this, &UActionBar::OnEquipmentUpdated);
}

void UActionBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UActionBar::NativeDestruct()
{
	if (m_OwnerLoadoutComponent.IsValid())
	{
		m_OwnerLoadoutComponent->OnSpellsUpdated.Remove(m_OnSpellsUpdatedHandled);
		m_OwnerLoadoutComponent->OnEquipmentUpdated.Remove(m_OnEquipmentUpdatedHandled);
	}

	Super::NativeDestruct();
}

void UActionBar::OnSpellsUpdated(const TArray<FGameplayTag>& spellTags)
{
	for(int i = 0; i < spellTags.Num(); ++i)
	{
		if (i == 0)
		{
			m_MainHandActionSlot1->SetSpellTag(spellTags[i]);
		}
		else if (i == 1)
		{
			m_MainHandActionSlot2->SetSpellTag(spellTags[i]);
		}
		else if (i == 2)
		{
			m_OffHandActionSlot1->SetSpellTag(spellTags[i]);
		}
		else if (i == 3)
		{
			m_OffHandActionSlot2->SetSpellTag(spellTags[i]);
		}
		else
		{
			LOG_ERROR("[ACTION BAR] Received more spell tags than action slots");
			return;
		}
	}
}

void UActionBar::OnEquipmentUpdated(const FEquipmentMap& equipmentMap)
{
	if(equipmentMap.Map.Contains(EEquippablePart::RIGHT_HAND))
	{
		FGameplayTag rightHandTag = equipmentMap.Map[EEquippablePart::RIGHT_HAND];

		m_MainHandActionSlot1->SetEquipmentTag(rightHandTag);
		m_MainHandActionSlot2->SetEquipmentTag(rightHandTag);
	}
	else
	{
		m_MainHandActionSlot1->SetEquipmentTag(FGameplayTag::EmptyTag);
		m_MainHandActionSlot2->SetEquipmentTag(FGameplayTag::EmptyTag);
	}

	if(equipmentMap.Map.Contains(EEquippablePart::LEFT_HAND))
	{
		FGameplayTag leftHandTag = equipmentMap.Map[EEquippablePart::LEFT_HAND];

		m_OffHandActionSlot1->SetEquipmentTag(leftHandTag);
		m_OffHandActionSlot2->SetEquipmentTag(leftHandTag);
	}
	else
	{
		m_OffHandActionSlot1->SetEquipmentTag(FGameplayTag::EmptyTag);
		m_OffHandActionSlot2->SetEquipmentTag(FGameplayTag::EmptyTag);
	}
}
