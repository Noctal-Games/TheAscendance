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
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Not necessary, will play before the Ability animations if set"))
	TSoftObjectPtr<UAnimMontage> TelegraphMontage = nullptr;
};