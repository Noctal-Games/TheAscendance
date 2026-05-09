// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractState.h"
#include "InvestigateState.generated.h"

class UHSMAgentComponent;
class UNavigationSystemV1;

UCLASS()
class THEASCENDANCE_API UInvestigateState : public UAbstractState
{
	GENERATED_BODY()
	
public:
	virtual void StartState(UHSMAgentComponent* owningAgent) override;
	virtual void Update(float deltaTime) override;
	virtual void EndState() override;

	virtual const FString GetStateToString() const override;

private:
	TWeakObjectPtr<UNavigationSystemV1> m_NavSystem = nullptr;
	float m_Timer = 0.0f;
	float m_DelayBetweenPathing = 0.0f;

	float m_InvestigationTimer = 0.0f;
};
