// Fill out your copyright notice in the Description page of Project Settings.


#include "AbstractCombatState.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/AI/Combat/Components/CombatAIComponent.h"

void UAbstractCombatState::Init(UCombatAIComponent* ownerComponent)
{
	m_OwnerComponent = ownerComponent;
}

const FString UAbstractCombatState::GetStateToString() const
{
	return FString("ABSTRACT COMBAT STATE - IMPLEMENT COMBAT STATE OVERRIDE");
}