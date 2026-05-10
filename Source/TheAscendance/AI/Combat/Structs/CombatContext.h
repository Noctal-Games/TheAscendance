// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CombatContext.generated.h"

USTRUCT()
struct FCombatContext
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<AActor> Target = nullptr;

	UPROPERTY()
	float DistanceToTarget = 0.0f;

	UPROPERTY()
	bool TargetDetected = false;

	UPROPERTY()
	bool HasLOS = false;
};
