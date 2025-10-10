// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellLoader.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/Subsystems/DataHandlerSubsystem.h"
#include "TheAscendance/Spells/SpellFactory.h"
#include "TheAscendance/Spells/Structs/SpellData.h"

void USpellLoader::Init()
{
	m_SpellFactory = MakeUnique<SpellFactory>();

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

ISpell* USpellLoader::CreateSpellFromID(int spellID, ISpellCaster* spellOwner)
{
	if (m_SpellTable == nullptr || m_SpellFactory == nullptr)
	{
		LOG_ERROR("SpellLoader tried to create Spell without a valid SpellTable or without a SpellFactory");
		return nullptr;
	}

	static const FString contextString(TEXT("Spell Context String"));

	TArray<FSpellTableData*> spellStructs;
	m_SpellTable->GetAllRows(contextString, spellStructs);

	for (const auto data : spellStructs)
	{
		if (data->SpellID != spellID)
		{
			continue;
		}

		FSoftObjectPath path(data->SpellData.ToSoftObjectPath());
		UObject* pathObject = path.ResolveObject();

		if (pathObject == nullptr)
		{
			pathObject = path.TryLoad();
		}

		if (pathObject == nullptr)
		{
			LOG_ERROR("SpellLoader failed to load SpellData for Spell ID: %i", spellID);
			return nullptr;
		}

		if (USpellData* spellData = Cast<USpellData>(pathObject))
		{
			return m_SpellFactory->CreateSpell(spellData, spellOwner);
		}
	}

	LOG_ERROR("SpellLoader failed to create Spell for Spell ID: %i", spellID);
	return nullptr;
}
