// Fill out your copyright notice in the Description page of Project Settings.


#include "TelegraphAttackState.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "AttackCombatState.h"
#include "TheAscendance/AI/Components/HSMAgentComponent.h"
#include "TheAscendance/Characters/Enemies/BaseEnemy.h"

void UTelegraphAttackState::StartState(UHSMAgentComponent* agent)
{
	UAbstractState::StartState(agent);

	if (m_Agent == nullptr || m_AttackCombatState.IsValid() == false)
	{
		return;
	}

	if(ABaseEnemy* enemy = m_Agent->GetAgentOwner())
	{
		if(m_AttackCombatState->m_CurrentTelegraphMontage.IsValid() == false)
		{
			LOG_ERROR("TelegraphAttackState received invalid TelegraphMontage");
			return;
		}

		enemy->PlayAnimationMontage(m_AttackCombatState->m_CurrentTelegraphMontage.Get());
	}
}

void UTelegraphAttackState::Update(float deltaTime)
{
}

void UTelegraphAttackState::EndState()
{
	UAbstractState::EndState();
}
