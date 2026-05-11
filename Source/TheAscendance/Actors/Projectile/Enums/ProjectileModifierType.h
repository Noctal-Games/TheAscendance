// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "ProjectileModifierType.generated.h"

UENUM(BlueprintType)
enum class EProjectileModifierType : uint8
{
	NONE,

	TRAIL UMETA(DisplayName = "Trail"),
	PENETRATION UMETA(DisplayName = "Penetration"),

	MAX UMETA(Hidden)
};