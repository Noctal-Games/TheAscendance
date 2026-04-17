// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "AbilityModifierType.generated.h"

UENUM(BlueprintType)
enum class EAbilityModifierType : uint8
{
	NONE,

	CHARGED UMETA(DisplayName = "Charged"),

	MAX UMETA(Hidden)
};
