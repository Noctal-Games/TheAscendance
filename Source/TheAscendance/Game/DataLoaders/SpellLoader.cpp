// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellLoader.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/GameplayTagHelpers.h"
#include "TheAscendance/Game/Subsystems/DataHandlerSubsystem.h"
#include "TheAscendance/Abilities/Spells/Structs/SpellData.h"

void USpellLoader::Init()
{
	if (UDataHandlerSubsystem* dataHandler = UCoreFunctionLibrary::GetDataHandlerSubsystem())
	{
		m_SpellTable = dataHandler->LoadData(EDataGroup::SPELLS);

		if (m_SpellTable == nullptr)
		{
			LOG_ERROR("SpellLoader failed to load Spell DataTable");
		}
		else
		{
			LOG_INFO("SpellLoader succeeded to load Spell DataTable");
		}
	}
}

//ISpell* USpellLoader::CreateSpellFromTag(const FGameplayTag& spellTag, ISpellCaster* spellOwner) const
//{
//	if (m_SpellTable == nullptr || m_SpellFactory == nullptr)
//	{
//		LOG_ERROR("[SPELL LOADER] Tried to CreateSpell without a valid SpellTable or without a SpellFactory");
//		return nullptr;
//	}
//
//	static const FString contextString(TEXT("Spell Context String"));
//
//	TArray<FSpellTableData*> spellStructs;
//	m_SpellTable->GetAllRows(contextString, spellStructs);
//
//	for (const auto data : spellStructs)
//	{
//		if (data->SpellTag != spellTag)
//		{
//			continue;
//		}
//
//		FSoftObjectPath path(data->SpellData.ToSoftObjectPath());
//		UObject* pathObject = path.ResolveObject();
//
//		if (pathObject == nullptr)
//		{
//			pathObject = path.TryLoad();
//		}
//
//		if (pathObject == nullptr)
//		{
//			LOG_ERROR("[SPELL LOADER] Failed to load SpellData for Spell: %s", *spellTag.ToString());
//			return nullptr;
//		}
//
//		if (USpellData* spellData = Cast<USpellData>(pathObject))
//		{
//			if(spellTag != spellData->SpellTag)
//			{
//				LOG_ERROR("[SPELL LOADER] SpellTag mismatch between SpellTable and SpellData for Spell: %s", *spellTag.ToString());
//			}
//
//			return m_SpellFactory->CreateSpell(spellData, spellOwner);
//		}
//	}
//
//	LOG_ERROR("[SPELL LOADER] Failed to create Spell for Spell: %s", *spellTag.ToString());
//	return nullptr;
//}

FSpellTableData* USpellLoader::GetSpellTableDataFromTag(const FGameplayTag& spellTag) const
{
	if (m_SpellTable == nullptr)
	{
		LOG_ERROR("[SPELL LOADER] Tried to GetSpellTableData without a valid SpellTable");
		return nullptr;
	}

	static const FString contextString(TEXT("Spell Context String"));

	TArray<FSpellTableData*> spellStructs;
	m_SpellTable->GetAllRows(contextString, spellStructs);

	for (const auto data : spellStructs)
	{
		if (data->SpellTag != spellTag)
		{
			continue;
		}

		return data;
	}

	LOG_ERROR("[SPELL LOADER] Failed to GetSpellTableData for Spell: %s", *spellTag.ToString());
	return nullptr;
}

const TArray<TSharedPtr<FSpellTableData>> USpellLoader::GetAllSpellTableDataEntries() const
{
	TArray<TSharedPtr<FSpellTableData>> toReturn;

	if (m_SpellTable == nullptr)
	{
		LOG_ERROR("[SPELL LOADER] Tried to GetSpellTableData without a valid SpellTable");
		return toReturn;
	}

	static const FString contextString(TEXT("Spell Context String"));

	TArray<FSpellTableData*> spellStructs;
	m_SpellTable->GetAllRows(contextString, spellStructs);

	for (const auto data : spellStructs)
	{
		toReturn.Add(MakeShared<FSpellTableData>(*data));
	}

	return toReturn;
}