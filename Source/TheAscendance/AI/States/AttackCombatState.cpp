// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackCombatState.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/AI/Combat/Components/CombatAIComponent.h"

void UAttackCombatState::StartState()
{
	if (m_OwnerComponent != nullptr)
	{
		m_OwnerComponent->SetIsCombatLocked(true);
		m_OwnerComponent->UseAbility();
	}
}

void UAttackCombatState::Update(float deltaTime)
{

}

void UAttackCombatState::EndState()
{
}

const FString UAttackCombatState::GetStateToString() const
{
	return FString("ATTACK COMBAT STATE");
}
