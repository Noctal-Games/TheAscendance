// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackCombatState.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/AI/Components/HSMAgentComponent.h"
#include "TheAscendance/Characters/Player/PlayerCharacter.h"
#include "EngageAttackState.h"
#include "TelegraphAttackState.h"
#include "ExecuteAttackState.h"
#include "CooldownAttackState.h"

void UAttackCombatState::StartState(UHSMAgentComponent* owningAgent)
{
	UAbstractState::StartState(owningAgent);

	if (m_Agent == nullptr)
	{
		return;
	}

	m_Agent->SetFocus(m_Agent->GetTargetPlayer());

	m_CurrentAttackState = EAttackState::MAX;
	m_AttackStates.Add(EAttackState::ENGAGE, NewObject<UEngageAttackState>());
	m_AttackStates.Add(EAttackState::TELEGRAPH, NewObject<UTelegraphAttackState>());
	m_AttackStates.Add(EAttackState::EXECUTE, NewObject<UExecuteAttackState>());
	m_AttackStates.Add(EAttackState::COOLDOWN, NewObject<UCooldownAttackState>());

	if (m_AttackStates.Num() != (int32)EState::MAX)
	{
		LOG_ERROR("AttackStates count does not match EAttackState length");
	}

	SetAttackState(EAttackState::ENGAGE);
}

void UAttackCombatState::Update(float deltaTime)
{
	if (m_AttackStates.Num() == 0 || m_AttackStates[m_CurrentAttackState] == nullptr)
	{
		return;
	}

	m_AttackStates[m_CurrentAttackState]->Update(deltaTime);
}

void UAttackCombatState::EndState()
{
	TArray<EAttackState> states;
	m_AttackStates.GetKeys(states);

	for (EAttackState state : states)
	{
		if (state == m_CurrentAttackState)
		{
			m_AttackStates[state]->EndState();
		}

		m_AttackStates[state]->MarkAsGarbage();
	}

	m_AttackStates.Empty();
}

void UAttackCombatState::SetAttackState(EAttackState newState)
{
	if (m_CurrentAttackState == newState)
	{
		return;
	}
	else if (m_AttackStates.Contains(newState) == false || m_AttackStates[newState] == nullptr)
	{
		LOG_ERROR("Tried to set AttackState to an invalid state");
		return;
	}

	if (m_AttackStates.Contains(m_CurrentAttackState) && m_AttackStates[m_CurrentAttackState] != nullptr)
	{
		m_AttackStates[m_CurrentAttackState]->EndState();
	}

	m_CurrentAttackState = newState;

	m_AttackStates[m_CurrentAttackState]->SetAttackCombatState(this);
	m_AttackStates[m_CurrentAttackState]->StartState(m_Agent.Get());
}
