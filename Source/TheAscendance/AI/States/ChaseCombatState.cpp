// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaseCombatState.h"
#include "TheAscendance/AI/Components/HSMAgentComponent.h"

void UChaseCombatState::StartState(UHSMAgentComponent* owningAgent)
{
	UAbstractState::StartState(owningAgent);

	if (m_Agent == nullptr)
	{
		return;
	}

	m_Target = m_Agent->GetTargetPlayer();
	//m_Agent->SetIsSprinting(true);

	m_EndChaseCooldown = 5.0f;
	m_ChaseTimer = m_EndChaseCooldown;
}

void UChaseCombatState::Update(float deltaTime)
{
	if (m_Agent == nullptr || m_Target.IsValid() == false)
	{
		return;
	}

	if (m_Agent->IsTargetDetected() == false)
	{
		m_ChaseTimer -= deltaTime;

		if (m_ChaseTimer > 0)
		{
			m_TargetLastKnownPos = m_Target->GetActorLocation();
		}
		else
		{
			m_Agent->SetLocationToInvestigate(m_TargetLastKnownPos);
			m_Agent->SetState(EState::INVESTIGATE);
		}
	}
	else
	{
		m_TargetLastKnownPos = m_Target->GetActorLocation();

		if (m_Agent->IsTargetInActionableRange(m_TargetLastKnownPos) == true)
		{
			m_Agent->SetCombatState(ECombatState::POSITION);
			return;
		}

		if (m_ChaseTimer != m_EndChaseCooldown)
		{
			m_ChaseTimer = m_EndChaseCooldown;
		}
	}

	m_Agent->SetDestination(m_TargetLastKnownPos);
}

void UChaseCombatState::EndState()
{
	if (m_Agent != nullptr)
	{
		//m_Agent->SetIsSprinting(false);
	}

	UAbstractState::EndState();
}

const FString UChaseCombatState::GetStateToString() const
{
	return FString("CHASE");
}
