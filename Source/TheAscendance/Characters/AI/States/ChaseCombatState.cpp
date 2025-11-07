// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaseCombatState.h"
#include "TheAscendance/Characters/AI/Components/HSMAgentComponent.h"

void UChaseCombatState::StartState(UHSMAgentComponent* owningAgent)
{
	UAbstractState::StartState(owningAgent);

	if (m_Agent == nullptr)
	{
		return;
	}

	//m_Agent->SetIsSprinting(true);

	m_EndChaseCooldown = 5.0f;
	m_ChaseTimer = m_EndChaseCooldown;
	//m_Agent->SetVisionStrength(0.8f);
}

void UChaseCombatState::Update(float deltaTime)
{
	//Chase Logic
}

void UChaseCombatState::EndState()
{
	if (m_Agent != nullptr)
	{
		//m_Agent->SetIsSprinting(false);
	}

	UAbstractState::EndState();
}
