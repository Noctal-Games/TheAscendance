// Fill out your copyright notice in the Description page of Project Settings.


#include "InvestigateState.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/AI/Components/HSMAgentComponent.h"
#include "TheAscendance/Characters/Enemies/BaseEnemy.h"
#include "NavigationSystem.h"

void UInvestigateState::StartState(UHSMAgentComponent* owningAgent)
{
	UAbstractState::StartState(owningAgent);

	if (m_Agent == nullptr)
	{
		return;
	}

	m_Agent->SetDestination(m_Agent->GetLocationToInvestigate());

	if (UWorld* worldContext = UCoreFunctionLibrary::GetGameWorld())
	{
		m_NavSystem = UNavigationSystemV1::GetNavigationSystem(worldContext);
		m_DelayBetweenPathing = 3.0f;
		m_Timer = m_DelayBetweenPathing;
	}
}

void UInvestigateState::Update(float deltaTime)
{
	if (m_Agent->HasLineOfSight() == true)
	{
		m_Agent->SetState(EState::COMBAT);
		return;
	}

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

	if (m_Timer > 0)
	{
		return;
	}

	if (const ABaseEnemy* owner = m_Agent->GetAgentOwner())
	{
		FNavLocation location;
		m_NavSystem->GetRandomPointInNavigableRadius(owner->GetActorLocation(), 3000, location);

		m_Agent->SetDestination(location);
		m_Timer = m_DelayBetweenPathing;
	}
}

void UInvestigateState::EndState()
{
	m_Agent->SetLocationToInvestigate(FVector::ZeroVector);

	UAbstractState::EndState();
}
