// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "EffectType.generated.h"

UENUM(BlueprintType)
enum class EEffectType : uint8
{
	INSTANT UMETA(DisplayName = "Instant"),
	OVERTIME UMETA(DisplayName = "Over Time"),
	DURATION UMETA(DisplayName = "Duration"),
	AOE UMETA(DisplayName = "Area of Effect"),
	AURA UMETA(DisplayName = "Aura"),
	CHAIN UMETA(DisplayName = "Chain"),

	MAX UMETA(Hidden)
};
