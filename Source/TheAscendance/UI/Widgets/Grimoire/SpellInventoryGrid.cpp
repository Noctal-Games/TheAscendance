// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellInventoryGrid.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "SpellInventoryEntry.h"
#include "TheAscendance/UI/Data/SpellDataEntryObject.h"
#include "TheAscendance/Spells/Structs/SpellData.h"

void USpellInventoryGrid::PopulateList(const TArray<TSharedPtr<FSpellTableData>>& spellList)
{
	ClearListItems();
	m_SpellList = spellList;
	UpdateGrid();
}

void USpellInventoryGrid::UpdateGrid()
{
	if(m_SpellList.Num() == 0)
	{
		LOG_WARNING("[SPELL INVENTORY GRID] Attempted to CreateGrid with empty spell list");
		return;
	}

	TArray<UObject*> entries;

	for(const TSharedPtr<FSpellTableData>& spellData : m_SpellList)
	{
		if (spellData == nullptr)
		{
			LOG_WARNING("[SPELL INVENTORY GRID] Attempted to create grid entry with null spell data");
			continue;
		}

		USpellDataEntryObject* obj = NewObject<USpellDataEntryObject>(this);

		obj->Data = *spellData;
		obj->Icon = spellData->SpellIcon.LoadSynchronous();

		entries.Add(obj);
	}

	SetListItems(entries);
}

//void USpellInventoryGrid::ClearGrid()
//{
//	m_SpellList.Empty();
//
//	for(USpellInventoryEntry* entry : m_EntryWidgets)
//	{
//		if (entry)
//		{
//			entry->RemoveFromParent();
//		}
//	}
//
//	m_EntryWidgets.Empty();
//}

//if(m_EntryDefault == nullptr)
//{
//	LOG_ERROR("[SPELL INVENTORY GRID] Attempted to CreateGrid without a valid Entry Default");
//	return;
//}

//if(USpellInventoryEntry* entry = CreateWidget<USpellInventoryEntry>(this, m_EntryDefault))
//{
//	entry->Init(spellData);
//	m_EntryWidgets.Add(entry);
//}
//else
//{
//	LOG_ERROR("[SPELL INVENTORY GRID] Failed to create Spell Inventory Entry widget");
//}