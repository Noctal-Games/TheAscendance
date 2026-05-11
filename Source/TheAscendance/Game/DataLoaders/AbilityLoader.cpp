// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityLoader.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/GameplayTagHelpers.h"
#include "TheAscendance/Game/Subsystems/DataHandlerSubsystem.h"
#include "TheAscendance/Abilities/Spells/Structs/SpellData.h"
#include "TheAscendance/Abilities/Melee/Structs/MeleeData.h"
#include "TheAscendance/Abilities/Interfaces/Ability.h"

void UAbilityLoader::Init()
{
	if (UDataHandlerSubsystem* dataHandler = UCoreFunctionLibrary::GetDataHandlerSubsystem())
	{
		m_SpellTable = dataHandler->LoadData(EDataGroup::SPELLS);

		if (m_SpellTable == nullptr)
		{
			LOG_ERROR("[ABILITY LOADER] Failed to load SpellDataTable");
		}
		else
		{
			LOG_INFO("[ABILITY LOADER] Succeeded to load SpellDataTable");
		}

		m_MeleeTable = dataHandler->LoadData(EDataGroup::MELEE);

		if (m_MeleeTable == nullptr)
		{
			LOG_ERROR("[ABILITY LOADER] Failed to load MeleeDataTable");
		}
		else
		{
			LOG_INFO("[ABILITY LOADER] Succeeded to load MeleeDataTable");
		}
	}
}

IAbility* UAbilityLoader::CreateAbilityFromTag(const FGameplayTag& abilityTag, UAbilityComponent* abilityOwner)
{ 
	return CreateAbilityFromData(GetAbilityData(abilityTag), abilityOwner);
}

IAbility* UAbilityLoader::CreateAbilityFromData(UAbilityData* abilityData, UAbilityComponent* abilityOwner)
{
	if (m_AbilityFactory == nullptr)
	{
		LOG_INFO("[ABILITY LOADER] AbilityFactory is invalid, attempting to create one");

		m_AbilityFactory = MakeUnique<AbilityFactory>();

		if (m_AbilityFactory == nullptr)
		{
			LOG_ERROR("[ABILITY LOADER] Tried to CreateAbilityFromTag without a valid AbilityFactory");
			return nullptr;
		}
	}

	return m_AbilityFactory->CreateAbility(abilityData, abilityOwner);
}

UAbilityData* UAbilityLoader::GetAbilityData(const FGameplayTag& abilityTag)
{
	const FGameplayTag spellTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Spell"));
	const FGameplayTag meleeTag = FGameplayTag::RequestGameplayTag(TEXT("Ability.Melee"));

	UAbilityData* abilityData = nullptr;

	if (abilityTag.MatchesTag(spellTag) == true)
	{
		abilityData = GetSpellAbilityDataFromTag(abilityTag);
	}
	else if (abilityTag.MatchesTag(meleeTag) == true)
	{
		abilityData = GetMeleeAbilityDataFromTag(abilityTag);
	}
	else
	{
		LOG_ERROR("[ABILITY LOADER] AbilityTag does not match any Ability.Type tags");
	}

	// Future Ability Types would be checked here with additional else if statements
	return abilityData;
}

FSpellTableData* UAbilityLoader::GetSpellTableDataFromTag(const FGameplayTag& spellTag) const
{
	if (m_SpellTable == nullptr)
	{
		LOG_ERROR("[ABILITY LOADER] Tried to GetSpellTableData without a valid SpellTable");
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

	LOG_ERROR("[ABILITY LOADER] Failed to GetSpellTableData for Spell: %s", *spellTag.ToString());
	return nullptr;
}

const TArray<TSharedPtr<FSpellTableData>> UAbilityLoader::GetAllSpellTableDataEntries() const
{
	TArray<TSharedPtr<FSpellTableData>> toReturn;

	if (m_SpellTable == nullptr)
	{
		LOG_ERROR("[ABILITY LOADER] Tried to GetSpellTableData without a valid SpellTable");
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

USpellData* UAbilityLoader::GetSpellAbilityDataFromTag(const FGameplayTag& abilityTag) const
{
	if (m_SpellTable == nullptr)
	{
		LOG_ERROR("[ABILITY LOADER] Tried to GetSpellAbilityData without a valid SpellTable");
		return nullptr;
	}

	static const FString contextString(TEXT("Spell Context String"));

	TArray<FAbilityTableData*> spellStructs;
	m_SpellTable->GetAllRows(contextString, spellStructs);

	for (const auto& data : spellStructs)
	{
		if (data->AbilityTag != abilityTag)
		{
			continue;
		}

		FSoftObjectPath path(data->AbilityData.ToSoftObjectPath());
		UObject* pathObject = path.ResolveObject();

		if (pathObject == nullptr)
		{
			pathObject = path.TryLoad();
		}

		if (pathObject == nullptr)
		{
			LOG_ERROR("[ABILITY LOADER] Failed to load SpellData for Spell: %s", *abilityTag.ToString());
			return nullptr;
		}

		if (USpellData* spellAbilityData = Cast<USpellData>(pathObject))
		{
			if(abilityTag != spellAbilityData->AbilityTag)
			{
				LOG_ERROR("[ABILITY LOADER] AbilityTag mismatch between SpellTable and SpellData for Spell: %s", *abilityTag.ToString());
			}

			return spellAbilityData;
		}
	}

	LOG_ERROR("[ABILITY LOADER] Failed to GetSpellAbilityData for Spell: %s", *abilityTag.ToString());
	return nullptr;
}

UMeleeData* UAbilityLoader::GetMeleeAbilityDataFromTag(const FGameplayTag& abilityTag) const
{
	if (m_MeleeTable == nullptr)
	{
		LOG_ERROR("[ABILITY LOADER] Tried to GetMeleeAbilityData without a valid MeleeTable");
		return nullptr;
	}

	static const FString contextString(TEXT("Melee Context String"));

	TArray<FAbilityTableData*> meleeStructs;
	m_MeleeTable->GetAllRows(contextString, meleeStructs);

	for (const auto& data : meleeStructs)
	{
		if (data->AbilityTag != abilityTag)
		{
			continue;
		}

		FSoftObjectPath path(data->AbilityData.ToSoftObjectPath());
		UObject* pathObject = path.ResolveObject();

		if (pathObject == nullptr)
		{
			pathObject = path.TryLoad();
		}

		if (pathObject == nullptr)
		{
			LOG_ERROR("[ABILITY LOADER] Failed to load MeleeData for Melee: %s", *abilityTag.ToString());
			return nullptr;
		}

		if (UMeleeData* meleeAbilityData = Cast<UMeleeData>(pathObject))
		{
			if (abilityTag != meleeAbilityData->AbilityTag)
			{
				LOG_ERROR("[ABILITY LOADER] AbilityTag mismatch between MeleeTable and MeleeData for Melee: %s", *abilityTag.ToString());
			}

			return meleeAbilityData;
		}
	}

	LOG_ERROR("[ABILITY LOADER] Failed to GetMeleeAbilityData for Melee: %s", *abilityTag.ToString());
	return nullptr;
}
