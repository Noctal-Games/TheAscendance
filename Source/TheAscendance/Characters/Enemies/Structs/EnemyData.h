// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Characters/Structs/LoadoutSlotData.h"
#include "EnemyData.generated.h"

USTRUCT(BlueprintType)
struct FEnemyStats
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float Health = 0.0f;

	//UPROPERTY(EditAnywhere)
	//float PhysicalAttack = 0.0f;

	//UPROPERTY(EditAnywhere)
	//float MagicAttack = 0.0f;

	//UPROPERTY(EditAnywhere)
	//float PhysicalResistance = 0.0f;

	//UPROPERTY(EditAnywhere)
	//float MagicResistance = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float WalkSpeed = 0.0f;
	UPROPERTY(EditDefaultsOnly)
	float SprintSpeedBonus = 0.0f;
	//UPROPERTY(EditDefaultsOnly)
	//float CrouchSpeedPenalty = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float PreferredRange = 0.0f;
	UPROPERTY(EditDefaultsOnly)
	float RetreatRange = 0.0f;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Higher value for stronger vision", ClampMin = 0.0f, ClampMax = 1.0f))
	float SightStrength = 1.0f;
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Higher value for stronger hearing", ClampMin = 0.0f, ClampMax = 1.0f))
	float HearingStrength = 1.0f;
};

USTRUCT(BlueprintType)
struct FSpellPair
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	int PrimarySpell = 0;
	UPROPERTY(EditDefaultsOnly)
	int SecondarySpell = 0;
};

USTRUCT(BlueprintType)
struct FEnemyEquipmentData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<FLoadoutSlotData> LoadoutData;
	UPROPERTY(EditDefaultsOnly)
	FSpellPair MainHandSpells;
	UPROPERTY(EditDefaultsOnly)
	FSpellPair OffHandSpells;
};

USTRUCT(BlueprintType)
struct FEnemyData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FEnemyStats EnemyStats;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FEnemyEquipmentData EnemyEquipment;
};

USTRUCT(BlueprintType)
struct FEnemyTableData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int EnemyID = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString EnemyName = "";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString EnemyDescription = "";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FEnemyData EnemyData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class USkeletalMesh> EnemyMesh = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<class UAnimBlueprintGeneratedClass> EnemyAnimationBP = nullptr;
};