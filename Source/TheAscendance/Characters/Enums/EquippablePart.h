// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EEquippablePart : uint8
{
	NONE UMETA(DisplayName = "None"),

	RIGHT_HAND UMETA(DisplayName = "Right/Main Hand"),
	LEFT_HAND UMETA(DisplayName = "Left/Off Hand"),

	MAX UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EEquippablePart, EEquippablePart::MAX)
