// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Spells/Enums/SpellType.h"
#include "SpellModifierData.h"
#include "Engine/DataAsset.h"
#include "InstancedStruct.h"
#include "SpellData.generated.h"

class UNiagaraSystem;

UCLASS(Abstract, BlueprintType)
class THEASCENDANCE_API USpellData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SpellCooldown = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ManaCost = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "The Niagara for the spell itself. Applied to the projectile for ProjectileSpells and played when casting LocalSpells"))
	TSoftObjectPtr<UNiagaraSystem> SpellNiagara = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FGenericSpellModifier>> SpellModifiers;

	ESpellType SpellType = ESpellType::PROJECTILE;
};

UCLASS(BlueprintType)
class THEASCENDANCE_API ULocalSpellData : public USpellData
{
	GENERATED_BODY()

public:
	ULocalSpellData()
	{
		SpellType = ESpellType::LOCAL;
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FRangedSpellModifier>> RangedSpellModifiers;
};

UCLASS(BlueprintType)
class THEASCENDANCE_API UProjectileSpellData : public URangedSpellData
{
	GENERATED_BODY()

public:
	UProjectileSpellData()
	{
		SpellType = ESpellType::PROJECTILE;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float ProjectileSpeed = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.1", ClampMax = "2.0"))
	float ProjectileSize = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsAffectedByGravity = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "1.0", ToolTip = "The projectiles gravity scale. 0 is no gravity, 1 is default.", EditCondition = "IsAffectedByGravity == true", EditConditionHides))
	float GravityScale = 1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FProjectileSpellModifier>> ProjectileSpellModifiers;
};

UCLASS(BlueprintType)
class THEASCENDANCE_API UHitscanSpellData : public URangedSpellData
{
	GENERATED_BODY()

public:
	UHitscanSpellData()
	{
		SpellType = ESpellType::HITSCAN;
	}
};
// RANGED SPELL TYPE DATA ASSETS

USTRUCT(BlueprintType)
struct FSpellTableData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int SpellID = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SpellName = "";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString SpellDescription = "";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<USpellData> SpellData = nullptr;
};