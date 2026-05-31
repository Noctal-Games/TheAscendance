// Fill out your copyright notice in the Description page of Project Settings.


#include "ApproachTargetTask.h"
#include "TheAscendance/Core/CoreMacros.h"

#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FApproachTargetTask::EnterState(FStateTreeExecutionContext& context, const FStateTreeTransitionResult& transition) const
{
	FInstanceDataType& data = context.GetInstanceData(*this);

	if (data.Target.IsValid())
	{
		LOG_ERROR("[APPROACH TARGET TASK] Target is invalid");
		return EStateTreeRunStatus::Failed;
	}

	const AActor* owner = Cast<AActor>(context.GetOwner());
	const AActor* target = data.Target.Get();

	const FVector ownerLocation = owner->GetActorLocation();
	const FVector targetLocation = target->GetActorLocation();

	const FVector directionToTarget = (targetLocation - ownerLocation).GetSafeNormal();
	data.DesiredLocation = targetLocation + (directionToTarget * 150.0f);

	return EStateTreeRunStatus::Succeeded;
}
