// Fill out your copyright notice in the Description page of Project Settings.


#include "InvestigateState.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Characters/AI/Components/HSMAgentComponent.h"
#include "NavigationSystem.h"

void UInvestigateState::StartState(UHSMAgentComponent* owningAgent)
{
	UAbstractState::StartState(owningAgent);

	if (m_Agent == nullptr)
	{
		return;
	}

	//m_Agent->SetDestination(m_Agent->GetInvestigationLocation());
	//m_Agent->SetVisionStrength(0.6f);

	if (UWorld* worldContext = UCoreFunctionLibrary::GetGameWorld())
	{
		m_NavSystem = UNavigationSystemV1::GetNavigationSystem(worldContext);
		m_DelayBetweenPathing = 3.0f;
		m_Timer = m_DelayBetweenPathing;
	}
}

void UInvestigateState::Update(float deltaTime)
{
	//if (m_Agent->HasLineOfSight() == true)
	//{
	//	m_Agent->SetState(EState::COMBAT);
	//	return;
	//}

	if (m_Agent->HasPath() == true)
	{
		return;
	}

	m_InvestigationTimer -= deltaTime;

	if (m_InvestigationTimer <= 0.0f)
	{
		m_Agent->SetState(EState::IDLE);
		return;
	}

	m_Timer -= deltaTime;

	//if (m_Timer <= 0)
	//{
	//	FNavLocation location;
	//	m_NavSystem->GetRandomPointInNavigableRadius(m_Agent->GetActorLocation(), 1500, location);

	//	m_Agent->SetDestination(location);
	//	m_Timer = m_DelayBetweenPathing;
	//}
}

void UInvestigateState::EndState()
{
	//m_Agent->SetLocationToInvestigate(FVector::Zero());

	UAbstractState::EndState();
}
