// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractState.h"
#include "PositionCombatState.generated.h"

class UHSMAgentComponent;

UCLASS()
class THEASCENDANCE_API UPositionCombatState : public UAbstractState
{
	GENERATED_BODY()
	
public:
	virtual void StartState(UHSMAgentComponent* owningAgent) override;
	virtual void Update(float deltaTime) override;
	virtual void EndState() override;

private:
	TWeakObjectPtr<AActor> m_Target = nullptr;

	float m_PlayerActionReactionTimer = 0.0f;
};
