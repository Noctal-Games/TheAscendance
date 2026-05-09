// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractState.h"
#include "ChaseCombatState.generated.h"

class UHSMAgentComponent;

UCLASS()
class THEASCENDANCE_API UChaseCombatState : public UAbstractState
{
	GENERATED_BODY()

public:
	virtual void StartState(UHSMAgentComponent* owningAgent) override;
	virtual void Update(float deltaTime) override;
	virtual void EndState() override;

	virtual const FString GetStateToString() const override;

private:
	TWeakObjectPtr<AActor> m_Target = nullptr;

	FVector m_TargetLastKnownPos = FVector::ZeroVector;

	float m_ChaseTimer = 0.0f;
	float m_EndChaseCooldown = 0.0f;
};
