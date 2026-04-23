// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemLoader.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Game/Subsystems/DataHandlerSubsystem.h"
#include "TheAscendance/Items/Structs/ItemData.h"
#include "TheAscendance/Items/Structs/WeaponData.h"

void UItemLoader::Init()
{
	if (UDataHandlerSubsystem* dataHandler = UCoreFunctionLibrary::GetDataHandlerSubsystem())
	{
		 m_ItemTable = dataHandler->LoadData(EDataGroup::ITEMS);

		if (m_ItemTable == nullptr)
		{
			LOG_ERROR("[ITEM LOADER] Failed to load Item DataTable");
		}
		else
		{
			LOG_INFO("[ITEM LOADER] Succeeded to load Item DataTable");
		}

		m_WeaponTable = dataHandler->LoadData(EDataGroup::WEAPONS);

		if (m_WeaponTable == nullptr)
		{
			LOG_ERROR("[ITEM LOADER] Failed to load Weapon DataTable");
		}
		else
		{
			LOG_INFO("[ITEM LOADER] Succeeded to load Weapon DataTable");
		}

		m_WeaponTypeData = &dataHandler->GetWeaponTypeMap();
	}
}

FItemData* UItemLoader::GetItemData(const FGameplayTag& itemTag)
{
	if (m_ItemTable == nullptr)
	{
		LOG_ERROR("[ITEM LOADER] Tried to load ItemData without a valid ItemTable");
		return nullptr;
	}

	static const FString contextString(TEXT("Item Context String"));

	TArray<FItemData*> itemStructs;
	m_ItemTable->GetAllRows(contextString, itemStructs);

	for (const auto data : itemStructs)
	{
		if (data->ItemTag == itemTag)
		{
			return data;
		}
	}

	//Check Equipment Table as well since some items have their data there instead of the item table
	LOG_ERROR("[ITEM LOADER] Could not load ItemData for Item: %s", *itemTag.ToString());
	return nullptr;
}

FEquippableItemData* UItemLoader::GetEquipmentData(const FGameplayTag& itemTag)
{
	if (m_EquipmentTable == nullptr)
	{
		LOG_ERROR("[ITEM LOADER] Tried to load EquipmentData without a valid EquipmentTable");
		return nullptr;
	}

	static const FString contextString(TEXT("Equipment Context String"));

	TArray<FEquippableItemData*> equipmentStructs;
	m_EquipmentTable->GetAllRows(contextString, equipmentStructs);

	for (const auto data : equipmentStructs)
	{
		if (data->ItemTag == itemTag)
		{
			return data;
		}
	}

	LOG_ERROR("[ITEM LOADER] Could not load EquipmentData for Item: %s", *itemTag.ToString());
	return nullptr;
}

FWeaponData* UItemLoader::GetWeaponData(const FGameplayTag& itemTag)
{
	if (m_WeaponTable == nullptr)
	{
		LOG_ERROR("[ITEM LOADER] Tried to load Equippable without a valid WeaponTable");
		return nullptr;
	}

	static const FString contextString(TEXT("Weapon Context String"));

	TArray<FWeaponData*> weaponStructs;
	m_WeaponTable->GetAllRows(contextString, weaponStructs);

	for (const auto weaponData : weaponStructs)
	{
		if (weaponData->ItemTag == itemTag)
		{
			return weaponData;
		}
	}

	LOG_ERROR("[ITEM LOADER] Could not load WeaponData for Item: %s", *itemTag.ToString());
	return nullptr;
}

const FWeaponTypeData* UItemLoader::GetWeaponTypeData(EWeaponType type)
{
	if (m_WeaponTypeData == nullptr || m_WeaponTypeData->Contains(type) == false)
	{
		LOG_ERROR("[ITEM LOADER] WeaponTypeData doesn't contain data for Weapon Type: %s", *UEnum::GetValueAsString(type));
		return nullptr;
	}

	return &(*m_WeaponTypeData)[type];
}
