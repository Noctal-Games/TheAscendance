// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Core/AbilityHelpers.h"
#include "AbilityInfo.generated.h"

USTRUCT()
struct FAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY()
	TSoftObjectPtr<UTexture2D> Icon;

	TArray<UAbilityHelpers::EAbilitySlot> SlotsUsed;
};