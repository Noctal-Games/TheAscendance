// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Characters/Enums/EquippablePart.h"
#include "LoadoutSlotData.generated.h"

USTRUCT(BlueprintType)
struct FLoadoutSlotData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ItemID = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEquippablePart EquippedPart = EEquippablePart::NONE;
};
