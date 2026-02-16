// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Characters/Enums/EquippablePart.h"
#include "GameplayTagContainer.h"
#include "LoadoutSlotData.generated.h"

USTRUCT(BlueprintType)
struct FLoadoutSlotData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Item.Equipment"))
	FGameplayTag ItemTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEquippablePart EquippedPart = EEquippablePart::NONE;
};
