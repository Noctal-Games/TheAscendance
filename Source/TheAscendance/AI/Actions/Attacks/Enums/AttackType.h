// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EMeleeAttackType : uint8
{
	QUICK_ATTACK UMETA(DisplayName = "Quick Attack"),
	HEAVY_ATTACK UMETA(DisplayName = "Heavy Attack"),
	LUNGE_ATTACK UMETA(DisplayName = "Lunge Attack")
};
