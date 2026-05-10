// Fill out your copyright notice in the Description page of Project Settings.


#include "AbstractAttackState.h"
#include "TheAscendance/Core/CoreMacros.h"

void UAbstractAttackState::SetAttackCombatState(UAttackCombatState* attackCombatState)
{
	m_AttackCombatState = attackCombatState;
}

const FString UAbstractAttackState::GetStateToString() const
{
	return FString("ABSTRACT ATTACK STATE - IMPLEMENT ATTACK STATE OVERRIDE");
}