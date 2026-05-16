// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Abilities/Enums/AbilitySlot.h"
#include "GameplayTagContainer.h"
#include "AbilityInfo.generated.h"

USTRUCT()
struct FAbilityInfo
{
	GENERATED_BODY()

	FGameplayTag Tag;
	float Cooldown = 0.0f;

	UPROPERTY()
	TSoftObjectPtr<UTexture2D> Icon = nullptr;

	TArray<EAbilitySlot> SlotsUsed;
};