// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "State.generated.h"

UENUM(BlueprintType)
enum class EState : uint8
{
	IDLE UMETA(DisplayName = "Idle"),
	INVESTIGATE UMETA(DisplayName = "Investigate"),
	COMBAT UMETA(DisplayName = "Combat"),

	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	CHASE UMETA(DisplayName = "Chase"),
	POSITION UMETA(DisplayName = "Position"),
	ATTACK UMETA(DisplayName = "Attack"),
	FLEE UMETA(DisplayName = "Flee"),

	MAX
};