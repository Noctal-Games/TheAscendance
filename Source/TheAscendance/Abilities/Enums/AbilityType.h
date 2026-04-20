// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "AbilityType.generated.h"

UENUM()
enum class EAbilityType : uint8
{
	NONE,
	PROJECTILE_SPELL,
	HITSCAN_SPELL,
	LOCAL_SPELL
};
