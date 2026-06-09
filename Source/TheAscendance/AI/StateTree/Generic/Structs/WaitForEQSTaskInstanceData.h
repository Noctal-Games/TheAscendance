// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "WaitForEQSTaskInstanceData.generated.h"

class ATAAIController;

USTRUCT()
struct FWaitForEQSTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Context")
	TWeakObjectPtr<ATAAIController> Controller = nullptr;

	UPROPERTY(EditAnywhere, Category = "Output")
	FVector ResultLocation;

	UPROPERTY(EditAnywhere, Category = "Context")
	TObjectPtr<AActor> QueryOwner = nullptr;

	UPROPERTY(EditAnywhere, Category = "Parameter")
	TObjectPtr<UEnvQuery> QueryTemplate;

	UPROPERTY(EditAnywhere, EditFixedSize, Category = "Parameter")
	TArray<FAIDynamicParam> QueryConfig;

	/** determines which item will be stored (All = only first matching) */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	TEnumAsByte<EEnvQueryRunMode::Type> RunMode = EEnvQueryRunMode::SingleResult;

	int32 RequestId = INDEX_NONE;

	float Duration = 0.0f;
};
