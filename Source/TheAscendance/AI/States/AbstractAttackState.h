// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractState.h"
#include "AbstractAttackState.generated.h"

class UAttackCombatState;

UCLASS(Abstract)
class THEASCENDANCE_API UAbstractAttackState : public UAbstractState
{
	GENERATED_BODY()
	
public:
	void SetAttackCombatState(UAttackCombatState* attackCombatState);
	virtual const FString GetStateToString() const;

protected:
	UPROPERTY()
	TWeakObjectPtr<UAttackCombatState> m_AttackCombatState = nullptr;
};
