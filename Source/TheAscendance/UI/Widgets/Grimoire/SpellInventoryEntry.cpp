// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellInventoryEntry.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Spells/Structs/SpellData.h"
#include "TheAscendance/UI/Data/SpellDataEntryObject.h"
#include "TheAscendance/Game/Subsystems/UIManagerSubsystem.h"
#include "Grimoire.h"

#include "Components/Image.h"

void USpellInventoryEntry::Init(USpellDataEntryObject* spellDataEntry)
{
	LOG_WARNING("[SPELL INVENTORY ENTRY] No need to call Init on this child class");
}

void USpellInventoryEntry::NativeOnListItemObjectSet(UObject* listItemObject)
{
	m_SpellDataEntry = Cast<USpellDataEntryObject>(listItemObject);

	if (m_SpellDataEntry == nullptr)
	{
		LOG_ERROR("[SPELL INVENTORY ENTRY] Failed to set list item object - Object was not of type USpellDataEntryObject");
		return;
	}

	if (m_SpellDataEntry->Icon != nullptr)
	{
		m_SpellIcon->SetBrushFromTexture(m_SpellDataEntry->Icon);
	}
	else
	{
		LOG_ERROR("[SPELL INVENTORY ENTRY] Spell Icon was invalid");
	}
}

//void USpellInventoryEntry::Init(const TSharedPtr<FSpellTableData> spellData)
//{
//	m_SpellData = spellData;
//
//	if(m_SpellData == nullptr)
//	{
//		LOG_ERROR("[SPELL INVENTORY ENTRY] Failed to initialize Spell Inventory Entry with null spell data");
//		return;
//	}
//
//	m_SpellIconTexture = m_SpellData->SpellIcon;
//
//	if (m_SpellIconTexture.IsNull() == false)
//	{
//		TWeakObjectPtr<USpellInventoryEntry> weakThis(this);
//
//		UCoreFunctionLibrary::RequestAsyncLoad(m_SpellIconTexture.ToSoftObjectPath(), [weakThis]()
//			{
//				if (weakThis.IsValid())
//				{
//					weakThis->SetIcon();
//				}
//			});
//	}
//}

//void USpellInventoryEntry::SetIcon()
//{
//	if (m_SpellIconTexture.Get() != nullptr)
//	{
//		m_SpellIcon->SetBrushFromTexture(m_SpellIconTexture.Get());
//	}
//	else
//	{
//		LOG_ERROR("[ACTION BAR ICON] Failed to set icon - Texture was invalid. Using default");
//	}
//}