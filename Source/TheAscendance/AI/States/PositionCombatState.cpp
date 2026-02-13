// Fill out your copyright notice in the Description page of Project Settings.


#include "PositionCombatState.h"
#include "TheAscendance/AI/Components/HSMAgentComponent.h"
#include "TheAscendance/Characters/Player/PlayerCharacter.h"

void UPositionCombatState::StartState(UHSMAgentComponent* owningAgent)
{
	UAbstractState::StartState(owningAgent);

	if (m_Agent == nullptr)
	{
		return;
	}

	m_Target = m_Agent->GetTargetPlayer();

	if (m_Target.IsValid() == true)
	{
		m_Agent->SetFocus(m_Target.Get());
	}

	m_PlayerActionReactionTimer = m_Agent->GetRandomCombatReactionTime();
}

void UPositionCombatState::Update(float deltaTime)
{
	if (m_Agent == nullptr || m_Target.IsValid() == false)
	{
		return;
	}

	//Uncomment when action logic is setup
	//if(m_PlayerActionReactionTimer > 0.0f)
	//{
	//	m_PlayerActionReactionTimer -= deltaTime;
	//	return;
	//}

	FVector targetLocation = m_Target->GetActorLocation();

	if(m_Agent->IsTargetInActionableRange(targetLocation) == false)
	{
		m_Agent->SetCombatState(ECombatState::CHASE);
		return;
	}

	FVector ownerLocation = m_Agent->GetOwner()->GetActorLocation();
	FVector moveToLocation = ownerLocation;

	if (m_Agent->IsTargetTooClose(targetLocation) == true)
	{
		m_Agent->SetCombatState(ECombatState::ATTACK);
		return;
		//moveToLocation -= (targetLocation - ownerLocation);

		//Setup too close options, will need movement component
		//Backstep
		//Strafe
		//Shove/Quick Attack
		//Guard Stance

	}
	else if (m_Agent->IsTargetTooFar(targetLocation) == true)
	{
		moveToLocation += (targetLocation - ownerLocation);

		//Setup too far options, will need movement component
		//Advance
		//Charge/Lunge
		//Temporary Sprinting
	}
	else
	{
		//Setup ideal range options (when not attacking)
		//Combat Idle Stance
		//Strafing/Micro Adjustments
		//Slow walk to player
		//Guard stance
		//Taunt animation/sounds
	}

	m_Agent->SetDestination(moveToLocation);

	//Uncomment when action logic is setup
	//m_PlayerActionReactionTimer = m_Agent->GetRandomCombatReactionTime();
}

void UPositionCombatState::EndState()
{
}
