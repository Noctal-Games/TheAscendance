// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractState.h"
#include "TheAscendance/AI/Enums/State.h"
#include "AttackCombatState.generated.h"

class UHSMAgentComponent;
class UAbstractAttackState;
class UEngageAttackState;
class UTelegraphAttackState;
class UAnimMontage;

UCLASS()
class THEASCENDANCE_API UAttackCombatState : public UAbstractState
{
	GENERATED_BODY()
	
public:
	virtual void StartState(UHSMAgentComponent* owningAgent) override;
	virtual void Update(float deltaTime) override;
	virtual void EndState() override;

	void SetAttackState(EAttackState newState);
	virtual const FString GetStateToString() const override;

protected:
	friend class UAbstractAttackState;
	friend class UEngageAttackState;
	friend class UTelegraphAttackState;
	friend class UExecuteAttackState;

	UPROPERTY()
	TSoftObjectPtr<UAnimMontage> m_CurrentTelegraphMontage;
	UPROPERTY()
	TSoftObjectPtr<UAnimMontage> m_CurrentAttackMontage;

private:
	UPROPERTY()
	TMap<EAttackState, TObjectPtr<UAbstractAttackState>> m_AttackStates;
	EAttackState m_CurrentAttackState = EAttackState::MAX;
};
