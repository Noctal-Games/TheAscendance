// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UObject/AssetRegistryTagsContext.h"
#include "TheAscendance/Items/Enums/EquipmentSlot.h"
#include "GameplayTagContainer.h"
#include "ItemData.generated.h"

class UMeleeData;

UCLASS(BlueprintType)
class THEASCENDANCE_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual void GetAssetRegistryTags(FAssetRegistryTagsContext context) const override;

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
class THEASCENDANCE_API UEquippableItemData : public UItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TArray<EEquipmentSlot> EquipmentSlotsUsed;
};

UCLASS(BlueprintType)
class THEASCENDANCE_API UWeaponItemData : public UEquippableItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UMeleeData> MainAbility = nullptr;
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UMeleeData> AltAbility = nullptr;
};