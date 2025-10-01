// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "SpellModifierType.generated.h"

UENUM(BlueprintType)
enum class EGenericSpellModifierType : uint8
{
	NONE,

	APPLY_CASTER_EFFECT UMETA(DisplayName = "Apply Effect to Caster"),
	AOE UMETA(DisplayName = "Area of Effect"),

	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ERangedSpellModifierType : uint8
{
	NONE,

	SCATTER UMETA(DisplayName = "Scatter"),
	KNOCKBACK UMETA(DisplayName = "Knockback"),

	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EProjectileSpellModifierType : uint8
{
	NONE,

	TRAIL UMETA(DisplayName = "Trail"),
	PENETRATION UMETA(DisplayName = "Penetration"),

	MAX UMETA(Hidden)
};