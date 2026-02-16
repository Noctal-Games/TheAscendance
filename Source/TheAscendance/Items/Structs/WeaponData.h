// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Items/Enums/WeaponType.h"
#include "GameplayTagContainer.h"
#include "WeaponData.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Item.Equipment.Weapon"))
	FGameplayTag ItemTag;
	UPROPERTY(EditDefaultsOnly)
	EWeaponType WeaponType = EWeaponType::HAND;

	FWeaponData() {}

	FWeaponData(FWeaponData& itemData) = default;
};

USTRUCT(BlueprintType)
struct FWeaponTypeData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	bool CastsSpell = false;
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "CastsSpell == true", EditConditionHides))
	bool CanToggleMelee = false;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "CastsSpell == false || CanToggleMelee == true", EditConditionHides))
	float PrimaryAttackMeleeDamage = 0.0f;
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "CastsSpell == false || CanToggleMelee == true", EditConditionHides))
	float PrimaryAttackStaminaCost = 0.0f;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "CastsSpell == false || CanToggleMelee == true", EditConditionHides))
	float AltAttackMeleeDamage = 0.0f;
	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "CastsSpell == false || CanToggleMelee == true", EditConditionHides))
	float AltAttackStaminaCost = 0.0f;

	FWeaponTypeData() {}

	FWeaponTypeData(const FWeaponTypeData& itemData) = default;
};