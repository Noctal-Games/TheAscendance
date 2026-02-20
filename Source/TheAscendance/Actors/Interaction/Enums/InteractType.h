// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "InteractType.generated.h"

UENUM(BlueprintType)
enum class EInteractType : uint8
{
	INTERACT UMETA(DisplayName = "Default"),
	PICKUP UMETA(DisplayName = "PickUp"),

	MAX UMETA(Hidden)
};

