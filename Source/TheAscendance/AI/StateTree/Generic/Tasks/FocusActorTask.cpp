// Fill out your copyright notice in the Description page of Project Settings.


#include "FocusActorTask.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/AI/TAAIController.h"

#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FFocusActorTask::EnterState(FStateTreeExecutionContext& context, const FStateTreeTransitionResult& transition) const
{
	FInstanceDataType& data = context.GetInstanceData(*this);

	if (data.Controller.IsValid() == false)
	{
		LOG_ERROR("[FOCUS ACTOR TASK] Controller is invalid");
		return EStateTreeRunStatus::Failed;
	}

	if (data.Target.IsValid() == false)
	{
		LOG_ERROR("[FOCUS ACTOR TASK] Target is invalid");
		return EStateTreeRunStatus::Failed;
	}

	ATAAIController* controller = data.Controller.Get();
	controller->SetFocus(data.Target.Get());

	return EStateTreeRunStatus::Running;
}
