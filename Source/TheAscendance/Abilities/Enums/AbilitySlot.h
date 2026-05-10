// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "AbilitySlot.generated.h"

UENUM(BlueprintType)
enum class EAbilitySlot : uint8
{
	MAINHAND_PRIMARY UMETA(DisplayName = "Mainhand Primary"),
	MAINHAND_ALT     UMETA(DisplayName = "Mainhand Alt"),
	OFFHAND_PRIMARY  UMETA(DisplayName = "Offhand Primary"),
	OFFHAND_ALT      UMETA(DisplayName = "Offhand Alt")
};