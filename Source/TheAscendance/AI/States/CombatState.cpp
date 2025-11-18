// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatState.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/AI/Components/HSMAgentComponent.h"
#include "ChaseCombatState.h"
#include "PositionCombatState.h"
#include "AttackCombatState.h"

void UCombatState::StartState(UHSMAgentComponent* owningAgent)
{
	UAbstractState::StartState(owningAgent);

	if (m_Agent == nullptr)
	{
		return;
	}

	m_CurrentCombatState = ECombatState::MAX;
	m_CombatStates.Add(ECombatState::CHASE, NewObject<UChaseCombatState>());
	m_CombatStates.Add(ECombatState::POSITION, NewObject<UPositionCombatState>());
	m_CombatStates.Add(ECombatState::ATTACK, NewObject<UAttackCombatState>());

	if (m_CombatStates.Num() != (int32)EState::MAX)
	{
		LOG_ERROR("CombatStates count does not match ECombatState length");
	}

	SetCombatState(ECombatState::CHASE);
}

void UCombatState::Update(float deltaTime)
{
	if (m_Agent->HasLineOfSight() == false)
	{
		m_Agent->SetState(EState::IDLE);
		return;
	}

	if (m_CombatStates.Num() == 0 || m_CombatStates[m_CurrentCombatState] == nullptr)
	{
		return;
	}

	m_CombatStates[m_CurrentCombatState]->Update(deltaTime);
}

void UCombatState::EndState()
{
	TArray<ECombatState> states;
	m_CombatStates.GetKeys(states);

	for (ECombatState state : states)
	{
		if (state == m_CurrentCombatState)
		{
			m_CombatStates[state]->EndState();
		}

		m_CombatStates[state]->MarkAsGarbage();
	}

	m_CombatStates.Empty();
}

void UCombatState::SetCombatState(ECombatState newState)
{
	if (m_CurrentCombatState == newState)
	{
		return;
	}
	else if (m_CombatStates.Contains(newState) == false || m_CombatStates[newState] == nullptr)
	{
		LOG_ERROR("Tried to set CombatState to an invalid state");
		return;
	}

	if (m_CombatStates.Contains(m_CurrentCombatState) && m_CombatStates[m_CurrentCombatState] != nullptr)
	{
		m_CombatStates[m_CurrentCombatState]->EndState();
	}

	m_CurrentCombatState = newState;

	m_CombatStates[m_CurrentCombatState]->StartState(m_Agent.Get());
}
