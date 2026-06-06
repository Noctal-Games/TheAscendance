// Fill out your copyright notice in the Description page of Project Settings.


#include "ClearFocusTask.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/AI/TAAIController.h"

#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FClearFocusTask::EnterState(FStateTreeExecutionContext& context, const FStateTreeTransitionResult& transition) const
{
	FInstanceDataType& data = context.GetInstanceData(*this);

	if (data.Controller.IsValid() == false)
	{
		LOG_ERROR("[CLEAR FOCUS TASK] Controller is invalid");
		return EStateTreeRunStatus::Failed;
	}

	ATAAIController* controller = data.Controller.Get();
	controller->ClearFocus(EAIFocusPriority::Gameplay);

	return EStateTreeRunStatus::Running;
}
