// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "TheAscendance/AI/StateTree/Generic/Structs/WaitForEQSTaskInstanceData.h"
#include "WaitForEQSTask.generated.h"

/**
 * 
 */
USTRUCT(meta = (DisplayName = "Wait For EQS"))
struct THEASCENDANCE_API FWaitForEQSTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()
	
	using FInstanceDataType = FWaitForEQSTaskInstanceData;

	virtual const UStruct* GetInstanceDataType() const override
	{
		return FWaitForEQSTaskInstanceData::StaticStruct();
	}

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& context, const FStateTreeTransitionResult& transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& context, const float deltaTime) const override;
	virtual void ExitState(FStateTreeExecutionContext& context, const FStateTreeTransitionResult& transition) const override;
};
