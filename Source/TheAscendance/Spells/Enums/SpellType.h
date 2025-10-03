// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "SpellType.generated.h"

UENUM(BlueprintType)
enum class ESpellType : uint8
{
	PROJECTILE UMETA(DisplayName = "Projectile"),
	HITSCAN UMETA(DisplayName = "Hitscan"),
	LOCAL UMETA(DisplayName = "Local"),

	MAX UMETA(Hidden)
};
