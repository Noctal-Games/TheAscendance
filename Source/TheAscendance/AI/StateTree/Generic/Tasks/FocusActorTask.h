// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "TheAscendance/AI/StateTree/Generic/Structs/FocusActorTaskInstanceData.h"
#include "FocusActorTask.generated.h"

USTRUCT(meta = (DisplayName = "Focus Actor", ToolTip = "Sets the Focus (LookAt) of the AI."))
struct THEASCENDANCE_API FFocusActorTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()
	
	using FInstanceDataType = FFocusActorTaskInstanceData;

	virtual const UStruct* GetInstanceDataType() const override
	{
		return FFocusActorTaskInstanceData::StaticStruct();
	}

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& context, const FStateTreeTransitionResult& transition) const override;
};
