// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Abilities/Structs/AbilityData.h"
#include "MeleeData.generated.h"

UCLASS(BlueprintType)
class THEASCENDANCE_API UMeleeData : public UAbilityData
{
	GENERATED_BODY()

public:
	UMeleeData()
	{
		AbilityType = EAbilityType::MELEE;
	}
};
