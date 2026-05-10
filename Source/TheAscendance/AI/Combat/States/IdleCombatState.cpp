// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleCombatState.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/AI/Combat/Components/CombatAIComponent.h"

void UIdleCombatState::StartState()
{
	UAbstractCombatState::StartState();
}

void UIdleCombatState::EndState()
{
}

void UIdleCombatState::Update(float deltaTime)
{
	if(m_OwnerComponent.IsValid() == false)
	{
		LOG_ERROR("[IDLE COMBAT STATE] OwnerComponent is invalid");
		return;
	}

	if(m_OwnerComponent->CombatContext.Target != nullptr)
	{
		m_OwnerComponent->SetState(ECombatState::APPROACH);
		return;
	}

	//Idle logic
}

const FString UIdleCombatState::GetStateToString() const
{
	return FString("IDLE COMBAT STATE");
}
