// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "SightBodyParts.generated.h"

UENUM(BlueprintType)
enum class ESightBodyParts : uint8
{
	HEAD UMETA(DisplayName = "Head"),
	CHEST UMETA(DisplayName = "Chest"),
	L_ARM UMETA(DisplayName = "Left Arm"),
	R_ARM UMETA(DisplayName = "Right Arm"),
	L_LEG UMETA(DisplayName = "Left Leg"),
	R_LEG UMETA(DisplayName = "Right Leg"),

	MAX UMETA(Hidden)
};

