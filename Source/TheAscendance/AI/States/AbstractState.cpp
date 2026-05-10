// Fill out your copyright notice in the Description page of Project Settings.


#include "AbstractState.h"
#include "TheAscendance/Core/CoreMacros.h"

void UAbstractState::StartState(UHSMAgentComponent* agent)
{
	if (agent == nullptr)
	{
		LOG_ERROR("Tried to start AbstractState with invalid agent");
		return;
	}

	m_Agent = agent;
}
void UAbstractState::EndState()
{
	if (m_Agent == nullptr)
	{
		LOG_ERROR("Tried to end AbstractState with invalid agent");
		return;
	}

	//m_Agent->SetDestination(agent->GetActorLocation());
}

const FString UAbstractState::GetStateToString() const
{
	return FString("ABSTRACT STATE - IMPLEMENT STATE OVERRIDE");
}
