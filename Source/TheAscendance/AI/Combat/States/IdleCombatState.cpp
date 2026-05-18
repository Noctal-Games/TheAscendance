// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleCombatState.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/AI/Combat/Components/CombatAIComponent.h"
#include "TheAscendance/AI/Combat/CombatManagerSubsystem.h"

void UIdleCombatState::StartState()
{
	UAbstractCombatState::StartState();

	if(UCombatManagerSubsystem* combatManager = UCoreFunctionLibrary::GetCombatManagerSubsystem())
	{
		//Unregister when idling, this state will be replaced with non-combat logic, so we don't need to be registered to the combat manager
		combatManager->UnRegisterAIComponent(m_OwnerComponent.Get());
	}
	else
	{
		LOG_ERROR("[IDLE COMBAT STATE] Failed to get CombatManagerSubsystem");
	}

	if (m_OwnerComponent != nullptr)
	{
		m_OwnerComponent->LookAtTarget();
		m_OwnerComponent->SetIsAIMovementPaused(true);

		//Strafe speed
	}
}

void UIdleCombatState::EndState()
{
	if (UCombatManagerSubsystem* combatManager = UCoreFunctionLibrary::GetCombatManagerSubsystem())
	{
		//Ending idle assumed entering a combat state for now, so register to combat manager
		combatManager->RegisterAIComponent(m_OwnerComponent.Get());
	}
	else
	{
		LOG_ERROR("[IDLE COMBAT STATE] Failed to get CombatManagerSubsystem");
	}

	if (m_OwnerComponent != nullptr)
	{
		m_OwnerComponent->SetIsAIMovementPaused(false);
		//Strafe speed
	}
}

void UIdleCombatState::Update(float deltaTime)
{
	if(m_OwnerComponent.IsValid() == false)
	{
		LOG_ERROR("[IDLE COMBAT STATE] OwnerComponent is invalid");
		return;
	}

	//Idle logic
}

const FString UIdleCombatState::GetStateToString() const
{
	return FString("IDLE COMBAT STATE");
}
