// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "FocusActorTaskInstanceData.generated.h"

class ATAAIController;

USTRUCT()
struct FClearFocusTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Context")
	TWeakObjectPtr<ATAAIController> Controller = nullptr;
};

USTRUCT()
struct FFocusActorTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Context")
	TWeakObjectPtr<ATAAIController> Controller = nullptr;

	UPROPERTY(EditAnywhere, Category = "Input")
	TWeakObjectPtr<AActor> Target = nullptr;
};
