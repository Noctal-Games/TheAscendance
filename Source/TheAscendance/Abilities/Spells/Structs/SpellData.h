// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Abilities/Spells/Enums/SpellType.h"
#include "TheAscendance/Abilities/Structs/AbilityData.h"
#include "TheAscendance/Actors/Projectile/Structs/ProjectileModifierData.h"
#include "SpellModifierData.h"
#include "SpellData.generated.h"

class UNiagaraSystem;

UCLASS(Abstract, BlueprintType)
class THEASCENDANCE_API USpellData : public UAbilityData
{
	GENERATED_BODY()

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ExcludeBaseStruct))
	//TArray<TInstancedStruct<FGenericSpellModifier>> SpellModifiers;
};

UCLASS(BlueprintType)
class THEASCENDANCE_API ULocalSpellData : public USpellData
{
	GENERATED_BODY()

public:
	ULocalSpellData()
	{
		AbilityType = EAbilityType::LOCAL_SPELL;
	}
};

// RANGED SPELL TYPE DATA ASSETS
UCLASS(Abstract, BlueprintType)
class THEASCENDANCE_API URangedSpellData : public USpellData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int HitDamage = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Range = 0.0f;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ExcludeBaseStruct))
	//TArray<TInstancedStruct<FRangedSpellModifier>> RangedSpellModifiers
};

UCLASS(BlueprintType)
class THEASCENDANCE_API UProjectileSpellData : public URangedSpellData
{
	GENERATED_BODY()

public:
	UProjectileSpellData()
	{
		AbilityType = EAbilityType::PROJECTILE_SPELL;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ProjectileSpeed = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.1", ClampMax = "2.0"))
	float ProjectileSize = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsAffectedByGravity = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "1.0", ToolTip = "The projectiles gravity scale. 0 is no gravity, 1 is default.", EditCondition = "IsAffectedByGravity == true", EditConditionHides))
	float GravityScale = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UNiagaraSystem> ProjectileNiagara = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FProjectileModifier>> ProjectileModifiers;
};

UCLASS(BlueprintType)
class THEASCENDANCE_API UHitscanSpellData : public URangedSpellData
{
	GENERATED_BODY()

public:
	UHitscanSpellData()
	{
		AbilityType = EAbilityType::HITSCAN_SPELL;
	}
};
// RANGED SPELL TYPE DATA ASSETS

USTRUCT(BlueprintType)
struct FSpellTableData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Ability.Spell"))
	FGameplayTag SpellTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SpellName = "";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString SpellDescription = "";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> SpellIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USpellData> SpellData = nullptr;
};