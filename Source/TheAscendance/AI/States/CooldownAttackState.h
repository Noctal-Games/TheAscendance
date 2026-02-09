// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractAttackState.h"
#include "CooldownAttackState.generated.h"

/**
 * 
 */
UCLASS()
class THEASCENDANCE_API UCooldownAttackState : public UAbstractAttackState
{
	GENERATED_BODY()
	
public:
	virtual void StartState(UHSMAgentComponent* agent) override;
	virtual void Update(float deltaTime) override;
	virtual void EndState() override;

private:
	FTimerHandle m_CooldownTimerHandle;
};
