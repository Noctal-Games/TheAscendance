// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EObjectiveType : uint8
{
	NONE UMETA(DisplayName = "None"),

	SINGLE UMETA(DisplayName = "Single"),
	BRANCHING UMETA(DisplayName = "Choice Branching"),
	MULTI UMETA(DisplayName = "Multi"),
	OPTIONAL UMETA(DisplayName = "Optional")
};

UENUM(BlueprintType)
enum class EObjectiveGoalType : uint8
{
	NONE UMETA(DisplayName = "None"),

	GATHER UMETA(DisplayName = "Gather"),
	TRAVEL_TO UMETA(DisplayName = "Travel To"),
	KILL_ANY UMETA(DisplayName = "Kill Any"),
	KILL_TYPE UMETA(DisplayName = "Kill Type"),
	INTERACT_WITH UMETA(DisplayName = "Interact With")
};
