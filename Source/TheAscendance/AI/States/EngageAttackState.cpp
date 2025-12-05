// Fill out your copyright notice in the Description page of Project Settings.


#include "EngageAttackState.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "AttackCombatState.h"
#include "TheAscendance/AI/Components/HSMAgentComponent.h"
#include "TheAscendance/AI/Actions/Attacks/Structs/AttackData.h"

void UEngageAttackState::StartState(UHSMAgentComponent* agent)
{
	UAbstractState::StartState(agent);

	if (m_Agent == nullptr || m_AttackCombatState.IsValid() == false)
	{
		return;
	}

	if(FQuickAttackData* attackData = Cast<FQuickAttackData>(m_Agent->GetMeleeAttackData(EMeleeAttackType::QUICK_ATTACK)))
	{
		m_AttackCombatState->m_CurrentTelegraphMontage = attackData->TelegraphMontage;

		UCoreFunctionLibrary::RequestAsyncLoad(attackData->AttackMontage.ToSoftObjectPath(), [this, attackData]()
			{
				m_AttackCombatState->SetAttackState(EAttackState::TELEGRAPH);
			}
		);
	}
	else
	{
		LOG_ERROR("EngageAttackState could not get QuickAttackData from HSMAgentComponent");
	}
}

void UEngageAttackState::Update(float deltaTime)
{
}

void UEngageAttackState::EndState()
{
	UAbstractState::EndState();
}
