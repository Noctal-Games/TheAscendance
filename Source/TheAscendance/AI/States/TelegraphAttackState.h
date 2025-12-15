// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractAttackState.h"
#include "TelegraphAttackState.generated.h"

/**
 * 
 */
UCLASS()
class THEASCENDANCE_API UTelegraphAttackState : public UAbstractAttackState
{
	GENERATED_BODY()
	
public:
	virtual void StartState(UHSMAgentComponent* agent) override;
	virtual void Update(float deltaTime) override;
	virtual void EndState() override;

private:
	void EndTelegraph();

private:
	float m_TelegraphTimer = 0.0f;

	FTimerHandle m_TelegraphTimerHandle;
};
