// Fill out your copyright notice in the Description page of Project Settings.


#include "TAAIController.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "Navigation/PathFollowingComponent.h"
#include "TheAscendance/AI/StateTree/Components/CustomStateTreeComponent.h"

#include "StateTree.h"

ATAAIController::ATAAIController() : AAIController()
{
	bStartAILogicOnPossess = true;

	m_StateTreeComponent = CreateDefaultSubobject<UCustomStateTreeComponent>(TEXT("StateTreeComponent"));
	checkf(m_StateTreeComponent, TEXT("StateTreeComponent failed to initialise"));
}

void ATAAIController::SetDestination(const FVector& destination, float acceptanceRadius)
{
	m_CurrentDestination = destination;

	FAIMoveRequest moveTo;
	moveTo.SetAcceptanceRadius(acceptanceRadius < 15.0f ? 15.0f : acceptanceRadius);
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

void ATAAIController::TestStartStateTree(UStateTree* stateTree)
{
	if (m_StateTreeComponent != nullptr)
	{
		m_StateTreeComponent->StartStateTree(stateTree);
	}
}

void ATAAIController::SendStateTreeEvent(const FStateTreeEvent& event)
{
	if (m_StateTreeComponent != nullptr)
	{
		m_StateTreeComponent->SendStateTreeEvent(event);
	}
}
