// Fill out your copyright notice in the Description page of Project Settings.


#include "TAAIController.h"
#include "Navigation/PathFollowingComponent.h"

void ATAAIController::SetDestination(const FVector& destination)
{
	m_CurrentDestination = destination;

	FAIMoveRequest moveTo;
	moveTo.SetAcceptanceRadius(50);
	moveTo.SetGoalLocation(m_CurrentDestination);

	MoveTo(moveTo);
}

void ATAAIController::ClearDestination()
{
	m_CurrentDestination = FVector::ZeroVector;
	StopMovement();
}

const FVector ATAAIController::GetCurrentDestination() const
{
	return m_CurrentDestination;
}

void ATAAIController::SetPauseMovement(bool val)
{
	if (val == true)
	{
		PauseMove(FAIRequestID::CurrentRequest);
		return;
	}

	ResumeMove(FAIRequestID::CurrentRequest);
}

bool ATAAIController::HasPath() const
{
	return IsFollowingAPath();
}