// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TheAscendance/Items/Enums/ItemType.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Items/Structs/WeaponData.h"
#include "TheAscendance/Items/Enums/EquipmentSlot.h"
#include "GameplayTagContainer.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Item"))
	FGameplayTag ItemTag;

	UPROPERTY(EditDefaultsOnly)
	FName ItemName = "";

	UPROPERTY(EditDefaultsOnly)
	FString ItemDescription = "";

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> ItemIcon = nullptr;

	UPROPERTY(EditDefaultsOnly)
	bool IsStackable = true;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UStaticMesh> ItemMesh = nullptr;

	FItemData() {}

	FItemData(FItemData& itemData) = default;
};

USTRUCT(BlueprintType)
struct FEquippableItemData : public FItemData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<EEquipmentSlot> EquipmentSlotsUsed;
};


UCLASS(BlueprintType)
class THEASCENDANCE_API UItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual void GetAssetRegistryTags(TArray<FAssetRegistryTag>& outTags) const override;

public:
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Item", AssetRegistrySearchable))
	FGameplayTag ItemTag;

	UPROPERTY(EditDefaultsOnly)
	FName ItemName = "";

	UPROPERTY(EditDefaultsOnly)
	FString ItemDescription = "";

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UTexture2D> ItemIcon = nullptr;

	UPROPERTY(EditDefaultsOnly)
	bool IsStackable = true;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UStaticMesh> ItemMesh = nullptr;
};

UCLASS(BlueprintType)
class THEASCENDANCE_API UEquippableItemDataAsset : public UItemDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<EEquipmentSlot> EquipmentSlotsUsed;
};

UCLASS(BlueprintType)
class THEASCENDANCE_API UWeaponItemDataAsset : public UEquippableItemDataAsset
{
	GENERATED_BODY()

public:
};