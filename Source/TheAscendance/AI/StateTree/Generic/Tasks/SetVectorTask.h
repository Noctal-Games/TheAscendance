// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "SetVectorTask.generated.h"

USTRUCT()
struct FSetVectorTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Input")
	FVector InVector;

	UPROPERTY(EditAnywhere, Category = "Input")
	FVector TargetVector;
};

USTRUCT(meta = (DisplayName = "Set Vector Task"))
struct THEASCENDANCE_API FSetVectorTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()
	
	using FInstanceDataType = FSetVectorTaskInstanceData;

	virtual const UStruct* GetInstanceDataType() const override
	{
		return FSetVectorTaskInstanceData::StaticStruct();
	}

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& context, const FStateTreeTransitionResult& transition) const override;
};
