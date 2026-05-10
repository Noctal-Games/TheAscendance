// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/AI/Actions/Attacks/Enums/AttackType.h"
#include "AttackData.generated.h"

class UAnimMontage;
class UAbilityData;

USTRUCT(BlueprintType)
struct FEnemyAbilityData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Higher weighted abilities will have a higher base priority. A higher SELF_HEAL ability will prioritise other SELF_ABILITIES"))
	float Weight = 1.0f;
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Goals used to help ability choice, HEAL_ALLY will take priority over DAMAGE if enemy stats align, as an example"))
	TArray<EAbilityGoal> Goals;
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UAbilityData> AbilityData = nullptr;
};

USTRUCT()
struct FLoadedAbilityData
{
	GENERATED_BODY()

	UPROPERTY()
	float Weight = 1.0f;
	UPROPERTY()
	FGameplayTag AbilityTag;
	UPROPERTY()
	TArray<EAbilityGoal> Goals;
	UPROPERTY()
	TObjectPtr<UAnimMontage> TelegraphMontage = nullptr;
};

USTRUCT()
struct FLoadedCombatSettings
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FLoadedAbilityData> Abilities;
	UPROPERTY()
	TMap<EAbilityGoal, float> GoalWeights;

	UPROPERTY()
	float PreferredEngagementRange = 0.0f;
	UPROPERTY()
	float MaxEngagementRange = 0.0f;
	UPROPERTY()
	float EngagementRangeTolerance = 0.1f;
};