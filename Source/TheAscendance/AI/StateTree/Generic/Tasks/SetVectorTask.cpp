// Fill out your copyright notice in the Description page of Project Settings.


#include "SetVectorTask.h"

#include "StateTreeAsyncExecutionContext.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FSetVectorTask::EnterState(FStateTreeExecutionContext& context, const FStateTreeTransitionResult& transition) const
{
	FInstanceDataType& instanceData = context.GetInstanceData(*this);
	instanceData.TargetVector = instanceData.InVector;

	return EStateTreeRunStatus::Running;
}