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

	const FAttackSetData* attackSetData = m_Agent->GetAttackSetData();

	if(attackSetData == nullptr)
	{
		LOG_ERROR("EngageAttackState could not get AttackSetData from HSMAgentComponent");
		return;
	}

	//Attack decision logic would go here to pick which attack to use

	if(const FQuickAttackData* attackData = &attackSetData->QuickAttackData)
	{
		m_AttackCombatState->m_CurrentTelegraphMontage = attackData->TelegraphMontage;
		m_AttackCombatState->m_CurrentAttackMontage = attackData->AttackMontage;

		if (m_AttackCombatState->m_CurrentTelegraphMontage.IsNull() == true)
		{
			LOG_ERROR("TelegraphMontage for QuickAttackData was invalid");
			return;
		}

		UCoreFunctionLibrary::RequestAsyncLoad(m_AttackCombatState->m_CurrentTelegraphMontage.ToSoftObjectPath(), [this, attackData]()
			{
				m_AttackCombatState->SetAttackState(EAttackState::TELEGRAPH);
			}
		);
	}
	else
	{
		LOG_ERROR("EngageAttackState could not get QuickAttackData from AttackSetData");
	}
}

void UEngageAttackState::Update(float deltaTime)
{
}

void UEngageAttackState::EndState()
{
	UAbstractState::EndState();
}

//if(FQuickAttackData* attackData = StaticCast<FQuickAttackData*>(m_Agent->GetMeleeAttackData(EMeleeAttackType::QUICK_ATTACK)))
//{
//	m_AttackCombatState->m_CurrentTelegraphMontage = attackData->TelegraphMontage;

//	UCoreFunctionLibrary::RequestAsyncLoad(attackData->AttackMontage.ToSoftObjectPath(), [this, attackData]()
//		{
//			m_AttackCombatState->SetAttackState(EAttackState::TELEGRAPH);
//		}
//	);
//}
//else
//{
//	LOG_ERROR("EngageAttackState could not get QuickAttackData from HSMAgentComponent");
//}