// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "AffectableStats.generated.h"

UENUM(BlueprintType)
enum class EOverTimeAffectableStat : uint8
{
	HEALTH UMETA(DisplayName = "Health"),
	MANA UMETA(DisplayName = "Mana"),
	STAMINA UMETA(DisplayName = "Stamina"),

	MAX UMETA(Hidden)
};
UENUM(BlueprintType)
enum class EDurationAffectableStat : uint8
{
	MAX_HEALTH UMETA(DisplayName = "Max Health"),
	MAX_MANA UMETA(DisplayName = "Max Mana"),
	MAX_STAMINA UMETA(DisplayName = "Max Stamina"),
	SHIELD UMETA(DisplayName = "Shield"),
	WALK_SPEED UMETA(DisplayName = "Speed (%)"),
	PHYSICAL_ATTACK UMETA(DisplayName = "Physical Attack (%)"),
	MAGIC_ATTACK UMETA(DisplayName = "Magic Attack (%)"),
	PHYSICAL_RESISTANCE UMETA(DisplayName = "Physical Resistance (%)"),
	MAGIC_RESISTANCE UMETA(DisplayName = "Magic Resistance (%)"),

	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EInstantAffectableStat : uint8
{
	HEALTH UMETA(DisplayName = "Health"),
	MANA UMETA(DisplayName = "Mana"),
	STAMINA UMETA(DisplayName = "Stamina"),
	SHIELD UMETA(DisplayName = "Shield"),

	MAX UMETA(Hidden)
};
