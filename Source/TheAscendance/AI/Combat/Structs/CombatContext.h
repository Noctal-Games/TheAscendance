// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CombatContext.generated.h"

USTRUCT()
struct FCombatContext
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Context")
	TWeakObjectPtr<AActor> Target = nullptr;
			
	UPROPERTY(EditAnywhere, Category = "Output")
	bool TargetDetected = false;

	UPROPERTY(EditAnywhere, Category = "Output")
	float DistanceToTarget = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Output")
	bool HasLOS = false;

	UPROPERTY(EditAnywhere, Category = "Output")
	bool TooClose = false;

	UPROPERTY(EditAnywhere, Category = "Output")
	bool TooFar = false;
};
