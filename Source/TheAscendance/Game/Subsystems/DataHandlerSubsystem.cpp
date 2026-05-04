// Fill out your copyright notice in the Description page of Project Settings.


#include "DataHandlerSubsystem.h"
#include "TheAscendance/Core/CoreMacros.h"

UDataTable* UDataHandlerSubsystem::LoadData(EDataGroup dataGroup)
{
	if (m_DataTables.Contains(dataGroup) == false)
	{
		LOG_ERROR("[DATA LOADER SUBSYSTEM] m_DataTables doesn't contain DataGroup: %s", *UEnum::GetValueAsString(dataGroup));
		return nullptr;
	}

	if (m_DataTables[dataGroup].IsValid() == true)
	{
		return m_DataTables[dataGroup].Get();
	}

	FSoftObjectPath path(m_DataTables[dataGroup].ToSoftObjectPath());
	UObject* pathObject = path.ResolveObject();

	if (pathObject == nullptr)
	{
		pathObject = path.TryLoad();
	}

	if (pathObject == nullptr)
	{
		LOG_ERROR("[DATA LOADER SUBSYSTEM] Failed to load DataTable for DataGroup: %s", *UEnum::GetValueAsString(dataGroup));
		return nullptr;
	}

	if (UDataTable* dataTable = Cast<UDataTable>(pathObject))
	{
		LOG_INFO("[DATA LOADER SUBSYSTEM] Succeeded at loading DataTable for DataGroup: %s", *UEnum::GetValueAsString(dataGroup));
		return dataTable;
	}

	LOG_ERROR("[DATA LOADER SUBSYSTEM] Failed to load DataTable for DataGroup: %s", *UEnum::GetValueAsString(dataGroup));
	return nullptr;
}

const TMap<EWeaponType, FWeaponTypeData>& UDataHandlerSubsystem::GetWeaponTypeMap()
{
	return m_WeaponTypeData;
}

void UDataHandlerSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);
}

bool UDataHandlerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (this->GetClass()->IsInBlueprint() && Super::ShouldCreateSubsystem(Outer))
	{
		return true;
	}

	return false;
}
