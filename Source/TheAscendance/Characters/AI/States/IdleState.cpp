// Fill out your copyright notice in the Description page of Project Settings.

#include "IdleState.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Characters/AI/Components/HSMAgentComponent.h"
#include "TheAscendance/Characters/Enemies/BaseEnemy.h"
#include "NavigationSystem.h"

void UIdleState::StartState(UHSMAgentComponent* agent)
{
	UAbstractState::StartState(agent);

	if (m_Agent == nullptr)
	{
		return;
	}

	if (UWorld* worldContext = UCoreFunctionLibrary::GetGameWorld())
	{
		m_NavigationSystem = UNavigationSystemV1::GetCurrent(worldContext);
	}
}

void UIdleState::Update(float deltaTime)
{
	if (m_Agent->HasPath() == true)
	{
		return;
	}

	if(const ABaseEnemy* owner = m_Agent->GetAgentOwner())
	{
		FNavLocation location;
		m_NavigationSystem->GetRandomPointInNavigableRadius(owner->GetActorLocation(), 3000, location);

		m_Agent->SetDestination(location);
	}
}

void UIdleState::EndState()
{
	if (m_Agent == nullptr)
	{
		return;
	}

	UAbstractState::EndState();
}