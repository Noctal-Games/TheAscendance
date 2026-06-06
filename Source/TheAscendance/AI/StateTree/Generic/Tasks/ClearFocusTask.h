// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "TheAscendance/AI/StateTree/Generic/Structs/FocusActorTaskInstanceData.h"
#include "ClearFocusTask.generated.h"

USTRUCT(meta = (DisplayName = "Clear Focus", ToolTip = "Clears the Focus (LookAt) of the AI."))
struct THEASCENDANCE_API FClearFocusTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()

	using FInstanceDataType = FClearFocusTaskInstanceData;

	virtual const UStruct* GetInstanceDataType() const override
	{
		return FClearFocusTaskInstanceData::StaticStruct();
	}

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& context, const FStateTreeTransitionResult& transition) const override;
};

