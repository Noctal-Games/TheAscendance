// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractState.h"
#include "TheAscendance/Characters/AI/Enums/State.h"
#include "CombatState.generated.h"

class UHSMAgentComponent;

UCLASS()
class THEASCENDANCE_API UCombatState : public UAbstractState
{
	GENERATED_BODY()

public:
	virtual void StartState(UHSMAgentComponent* owningAgent) override;
	virtual void Update(float deltaTime) override;
	virtual void EndState() override;

	void SetCombatState(ECombatState newState);

private:
	UPROPERTY()
	TMap<ECombatState, TObjectPtr<UAbstractState>> m_CombatStates;
	ECombatState m_CurrentCombatState = ECombatState::MAX;
};
