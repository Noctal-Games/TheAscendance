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
