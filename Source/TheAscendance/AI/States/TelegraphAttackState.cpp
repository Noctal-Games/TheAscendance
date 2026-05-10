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

	const FLoadedAbilityData* ability = nullptr;// m_AttackCombatState->m_CurrentAbilityData;

	if (ability == nullptr)
	{
		LOG_ERROR("[TELEGRAPH ATTACK STATE] Started with invalid ability data");
		return;
	}

	if (ability->TelegraphMontage == nullptr)
	{
		//Not necessary if no telegraph is expected
		EndTelegraph();
		return;
	}

	if (ABaseEnemy* enemy = m_Agent->GetAgentOwner())
	{
		//m_Agent->SetFocus(m_Agent->GetTargetPlayer());

		m_TelegraphTimer = enemy->PlayAnimationMontage(ability->TelegraphMontage);

		if (UWorld* worldContext = UCoreFunctionLibrary::GetGameWorld())
		{
			worldContext->GetTimerManager().SetTimer(m_TelegraphTimerHandle, this, &UTelegraphAttackState::EndTelegraph, m_TelegraphTimer, true);
		}
	}
}

void UTelegraphAttackState::Update(float deltaTime)
{
	UAbstractState::Update(deltaTime);
}

void UTelegraphAttackState::EndState()
{
	//m_Agent->ClearFocus();

	UAbstractState::EndState();
}

const FString UTelegraphAttackState::GetStateToString() const
{
	return FString("TELEPGRAPH ATTACK STATE");
}

void UTelegraphAttackState::EndTelegraph()
{
	if(m_AttackCombatState.IsValid() == false)
	{
		return;
	}

	//m_AttackCombatState->SetAttackState(EAttackState::EXECUTE);
}
