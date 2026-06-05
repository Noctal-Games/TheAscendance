// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MovementTaskInstanceData.generated.h"

class ATAAIController;

USTRUCT()
struct FMovementTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Context")
	TWeakObjectPtr<ATAAIController> Controller = nullptr;

	UPROPERTY(EditAnywhere, Category = "Context")
	TWeakObjectPtr<AActor> OwnerActor = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TWeakObjectPtr<AActor> Target = nullptr;

	UPROPERTY(EditAnywhere, Category = "Output")
	FVector DesiredLocation = FVector::ZeroVector;
};

USTRUCT()
struct FStrafeMovementTaskInstanceData : public FMovementTaskInstanceData
{
	GENERATED_BODY()

	float StrafeDuration = 0.0f;
	float StrafeDirection = 0.0f;
};