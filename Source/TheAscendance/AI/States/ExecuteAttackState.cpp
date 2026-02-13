// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecuteAttackState.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "AttackCombatState.h"
#include "TheAscendance/AI/Components/HSMAgentComponent.h"
#include "TheAscendance/Characters/Enemies/BaseEnemy.h"

void UExecuteAttackState::StartState(UHSMAgentComponent* agent)
{
	UAbstractState::StartState(agent);

	if (m_Agent == nullptr || m_AttackCombatState.IsValid() == false)
	{
		return;
	}

	if (ABaseEnemy* enemy = m_Agent->GetAgentOwner())
	{
		if (m_AttackCombatState->m_CurrentAttackMontage.IsValid() == false)
		{
			LOG_ERROR("ExecuteAttackState received invalid AttackMontage");
			return;
		}

		float attackTimer = enemy->PlayAnimationMontage(m_AttackCombatState->m_CurrentAttackMontage.Get());

		if (UWorld* worldContext = UCoreFunctionLibrary::GetGameWorld())
		{
			worldContext->GetTimerManager().SetTimer(m_ExecuteTimerHandle, this, &UExecuteAttackState::EndAttack, attackTimer, true);
		}
	}
}

void UExecuteAttackState::Update(float deltaTime)
{
}

void UExecuteAttackState::EndState()
{
	UAbstractState::EndState();
}

void UExecuteAttackState::EndAttack()
{
	if (m_AttackCombatState.IsValid() == false)
	{
		return;
	}

	m_Agent->SetCombatState(ECombatState::POSITION);
}