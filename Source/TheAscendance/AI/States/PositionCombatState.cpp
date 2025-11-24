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

	m_Agent->GetPreferredDistanceValues(m_PreferredDistanceFromTarget, m_PreferredDistanceTolerance);

	m_Target = m_Agent->GetTargetPlayer();

	if (m_Target.IsValid() == true)
	{
		m_Agent->SetFocus(m_Target.Get());
	}
}

void UPositionCombatState::Update(float deltaTime)
{
}

void UPositionCombatState::EndState()
{
}
