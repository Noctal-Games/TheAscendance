// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MovementTaskInstanceData.generated.h"

USTRUCT()
struct FMovementTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input")
	TWeakObjectPtr<AActor> Target = nullptr;

	UPROPERTY(EditAnywhere, Category = "Output")
	FVector DesiredLocation = FVector::ZeroVector;
};
