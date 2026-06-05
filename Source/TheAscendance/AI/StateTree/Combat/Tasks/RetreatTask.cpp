// Fill out your copyright notice in the Description page of Project Settings.


#include "RetreatTask.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/AI/TAAIController.h"

#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FRetreatTask::EnterState(FStateTreeExecutionContext& context, const FStateTreeTransitionResult& transition) const
{
	FInstanceDataType& data = context.GetInstanceData(*this);

	if (data.OwnerActor.IsValid() == false)
	{
		LOG_ERROR("[RETREAT TASK] OwnerActor is invalid");
		return EStateTreeRunStatus::Failed;
	}

	if (data.Target.IsValid() == false)
	{
		LOG_ERROR("[RETREAT TASK] Target is invalid");
		return EStateTreeRunStatus::Failed;
	}

	const AActor* owner = data.OwnerActor.Get();
	const AActor* target = data.Target.Get();

	const FVector ownerLocation = owner->GetActorLocation();
	const FVector targetLocation = target->GetActorLocation();

	FVector toTarget = targetLocation - ownerLocation;
	toTarget.Z = 0.0f;
	toTarget.Normalize();

	const FVector retreatDirection = -toTarget;

	constexpr float RetreatDistance = 150.0f;

	data.DesiredLocation = ownerLocation + (retreatDirection * RetreatDistance);

	return EStateTreeRunStatus::Succeeded;
}
