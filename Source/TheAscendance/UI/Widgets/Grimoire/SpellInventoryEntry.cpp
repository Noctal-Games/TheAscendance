// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellInventoryEntry.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Spells/Structs/SpellData.h"
#include "TheAscendance/UI/Data/SpellDataEntryObject.h"

#include "Components/Image.h"

void USpellInventoryEntry::NativeOnListItemObjectSet(UObject* listItemObject)
{
	USpellDataEntryObject* data = Cast<USpellDataEntryObject>(listItemObject);

	if (!data)
	{
		return;
	}

	if (data->Icon)
	{
		m_SpellIcon->SetBrushFromTexture(data->Icon);
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