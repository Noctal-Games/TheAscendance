// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "TheAscendance/AI/StateTree/Generic/Structs/MovementTaskInstanceData.h"
#include "RetreatTask.generated.h"

USTRUCT(meta = (DisplayName = "Retreat"))
struct THEASCENDANCE_API FRetreatTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FMovementTaskInstanceData;

	virtual const UStruct* GetInstanceDataType() const override
	{
		return FMovementTaskInstanceData::StaticStruct();
	}

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& context, const FStateTreeTransitionResult& transition) const override;
};