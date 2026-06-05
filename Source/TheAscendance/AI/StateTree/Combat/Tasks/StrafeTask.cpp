// Fill out your copyright notice in the Description page of Project Settings.


#include "StrafeTask.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/AI/TAAIController.h"
#include "TheAscendance/AI/AIEventGameplayTags.h"

#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FStrafeTask::EnterState(FStateTreeExecutionContext& context, const FStateTreeTransitionResult& transition) const
{
	FInstanceDataType& data = context.GetInstanceData(*this);

	if (data.StrafeDuration > 0.0f)
	{
		return EStateTreeRunStatus::Running;
	}

	if (data.OwnerActor.IsValid() == false)
	{
		LOG_ERROR("[STRAFE TASK] OwnerActor is invalid");
		return EStateTreeRunStatus::Failed;
	}

	if (data.Target.IsValid() == false)
	{
		LOG_ERROR("[STRAFE TASK] Target is invalid");
		return EStateTreeRunStatus::Failed;
	}

	data.StrafeDirection = FMath::RandBool() ? 1.0f : -1.0f;
	data.StrafeDuration = FMath::RandRange(1.5f, 3.5f);
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus FStrafeTask::Tick(FStateTreeExecutionContext& context, const float deltaTime) const
{
	Super::Tick(context, deltaTime);

	FInstanceDataType& data = context.GetInstanceData(*this);

	data.StrafeDuration -= deltaTime;

	if (data.StrafeDuration <= 0.0f)
	{
		if (data.Controller.IsValid() == false)
		{
			LOG_ERROR("[STRAFE TASK] Controller is invalid");
			return EStateTreeRunStatus::Failed;
		}

		ATAAIController* controller = data.Controller.Get();

		FStateTreeEvent event;
		event.Tag = EVENT_STRAFE_COMPLETE;
		
		controller->SendStateTreeEvent(event);
		return EStateTreeRunStatus::Succeeded;
	}

	const AActor* owner = data.OwnerActor.Get();
	const AActor* target = data.Target.Get();

	const FVector ownerLocation = owner->GetActorLocation();
	const FVector targetLocation = target->GetActorLocation();

	FVector toTarget = (targetLocation - ownerLocation);
	toTarget.Z = 0.0f;
	toTarget.Normalize();

	// Perpendicular vector for circling
	FVector rightVector = FVector::CrossProduct(FVector::UpVector, toTarget);
	rightVector.Normalize();

	FVector desiredDirection = rightVector * data.StrafeDirection;

	const float distance = FVector::Distance(ownerLocation, targetLocation);
	constexpr float preferredRange = 1000.0f;
	constexpr float tolerance = 0.1f;

	//m_OwnerComponent->GetCombatRangeValues(preferredRange, maxRange, tolerance);

	const float rangeDelta = distance - preferredRange;

	const float distanceRatio = FMath::Abs(rangeDelta) / FMath::Max(preferredRange, 1.0f);
	const float rangeUrgency = FMath::Clamp(distanceRatio, 0.0f, 1.0f);

	const float aggression = 0.5f;
	const float defensiveWeight = 1.0f - aggression;

	const float rangeSensitivity = 1.0f - tolerance;
	const float rangeCorrectionStrength = rangeUrgency * rangeSensitivity * defensiveWeight;

	// Too far
	if (rangeDelta > 0.0f)
	{
		desiredDirection += toTarget * rangeCorrectionStrength;
	}
	// Too close
	else
	{
		desiredDirection -= toTarget * rangeCorrectionStrength;
	}

	desiredDirection.Normalize();
	data.DesiredLocation = ownerLocation + (desiredDirection * 250.0f);

	return EStateTreeRunStatus::Running;
}

