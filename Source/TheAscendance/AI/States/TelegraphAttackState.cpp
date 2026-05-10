// Fill out your copyright notice in the Description page of Project Settings.


#include "TelegraphAttackState.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/StreamableFunctionLibrary.h"
#include "AttackCombatState.h"
#include "TheAscendance/AI/Components/HSMAgentComponent.h"
#include "TheAscendance/Characters/Enemies/BaseEnemy.h"
#include "TheAscendance/Characters/Player/PlayerCharacter.h"

void UTelegraphAttackState::StartState(UHSMAgentComponent* agent)
{
	UAbstractState::StartState(agent);

	if (m_Agent == nullptr || m_AttackCombatState.IsValid() == false)
	{
		return;
	}

	if (ABaseEnemy* enemy = m_Agent->GetAgentOwner())
	{
		if (m_AttackCombatState->m_CurrentTelegraphMontage.IsValid() == false)
		{
			LOG_ERROR("TelegraphAttackState received invalid TelegraphMontage");
			return;
		}

		//m_Agent->SetFocus(m_Agent->GetTargetPlayer());

		m_TelegraphTimer = enemy->PlayAnimationMontage(m_AttackCombatState->m_CurrentTelegraphMontage.Get());

		if (m_AttackCombatState->m_CurrentAttackMontage.IsNull() == true)
		{
			LOG_ERROR("AttackMontage for QuickAttackData was invalid");
		}
		else
		{
			UStreamableFunctionLibrary::RequestAsyncLoad(m_AttackCombatState->m_CurrentAttackMontage.ToSoftObjectPath());
		}

		if (UWorld* worldContext = UCoreFunctionLibrary::GetGameWorld())
		{
			worldContext->GetTimerManager().SetTimer(m_TelegraphTimerHandle, this, &UTelegraphAttackState::EndTelegraph, m_TelegraphTimer, true);
		}
	}
}

void UTelegraphAttackState::Update(float deltaTime)
{
}

void UTelegraphAttackState::EndState()
{
	//m_Agent->ClearFocus();

	UAbstractState::EndState();
}

void UTelegraphAttackState::EndTelegraph()
{
	if(m_AttackCombatState.IsValid() == false)
	{
		return;
	}

	m_AttackCombatState->SetAttackState(EAttackState::EXECUTE);
}
