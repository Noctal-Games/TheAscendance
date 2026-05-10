// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Items/Structs/WeaponData.h"
#include "TheAscendance/Items/Enums/WeaponType.h"
#include "GameplayTagContainer.h"
#include "ItemLoader.generated.h"

struct FItemData;
struct FEquippableItemData;
struct FWeaponData;

UCLASS()
class THEASCENDANCE_API UItemLoader : public UObject
{
	GENERATED_BODY()

public:
	void Init();

	FItemData* GetItemData(const FGameplayTag& itemTag);
	FEquippableItemData* GetEquipmentData(const FGameplayTag& itemTag);
	FWeaponData* GetWeaponData(const FGameplayTag& itemTag);
	const FWeaponTypeData* GetWeaponTypeData(EWeaponType type);

private:
	UPROPERTY()
	TObjectPtr<UDataTable> m_ItemTable = nullptr;
	UPROPERTY()
	TObjectPtr<UDataTable> m_EquipmentTable = nullptr;

	UPROPERTY()
	TObjectPtr<UDataTable> m_WeaponTable = nullptr;

	const TMap<EWeaponType, FWeaponTypeData>* m_WeaponTypeData;
};
