// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TheAscendance/Items/Structs/WeaponData.h"
#include "TheAscendance/Items/Enums/WeaponType.h"
#include "DataHandlerSubsystem.generated.h"

UENUM()
enum class EDataGroup : uint8
{
	ENEMIES,
	MELEE,
	SPELLS,
	EFFECTS
};

UCLASS(Blueprintable)
class THEASCENDANCE_API UDataHandlerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UDataTable* LoadData(EDataGroup dataGroup);

	const TMap<EWeaponType, FWeaponTypeData>& GetWeaponTypeMap();

	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Data Tables"))
	TMap<EDataGroup, TSoftObjectPtr<UDataTable>> m_DataTables;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Weapon Type Data Map"))
	TMap<EWeaponType, FWeaponTypeData> m_WeaponTypeData;
};
