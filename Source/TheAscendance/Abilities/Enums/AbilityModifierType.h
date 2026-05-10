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
	AOE UMETA(DisplayName = "Area of Effect"),
	APPLY_EFFECT UMETA(DisplayName = "Apply Effect"),
	KNOCKBACK UMETA(DisplayName = "Knockback"),
	APPLY_USER_EFFECT UMETA(DisplayName = "Apply User Effect"),
	SPAWN_EFFECT UMETA(DisplayName = "Spawn Effect"),

	MAX UMETA(Hidden)
};
