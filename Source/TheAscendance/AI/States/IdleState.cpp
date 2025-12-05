// Fill out your copyright notice in the Description page of Project Settings.

#include "IdleState.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/AI/Navigation/WaypointRoute.h"
#include "TheAscendance/AI/Components/HSMAgentComponent.h"
#include "TheAscendance/Characters/Enemies/BaseEnemy.h"
#include "TheAscendance/Characters/BaseCharacter.h"
#include "NavigationSystem.h"

void UIdleState::StartState(UHSMAgentComponent* agent)
{
	UAbstractState::StartState(agent);

	if (m_Agent == nullptr)
	{
		return;
	}

	if(AWaypointRoute* waypointRoute = m_Agent->GetWaypointRoute())
	{
		m_UsingWaypointRoute = true;
		waypointRoute->AddCharacterToRoute(m_Agent->GetAgentOwner());
		return;
	}

	if (UWorld* worldContext = UCoreFunctionLibrary::GetGameWorld())
	{
		m_NavigationSystem = UNavigationSystemV1::GetCurrent(worldContext);
	}
}

void UIdleState::Update(float deltaTime)
{
	if (m_Agent->HasLineOfSight() == true)
	{
		m_Agent->SetState(EState::COMBAT);
		return;
	}

	if (m_Agent->HasPath() == true || m_UsingWaypointRoute == true)
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

	if (AWaypointRoute* waypointRoute = m_Agent->GetWaypointRoute())
	{
		waypointRoute->RemoveCharacterFromRoute(m_Agent->GetAgentOwner());
	}

	m_UsingWaypointRoute = false;
	UAbstractState::EndState();
}