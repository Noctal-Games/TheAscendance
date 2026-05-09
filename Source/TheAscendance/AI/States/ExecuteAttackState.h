// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractAttackState.h"
#include "ExecuteAttackState.generated.h"

UCLASS()
class THEASCENDANCE_API UExecuteAttackState : public UAbstractAttackState
{
	GENERATED_BODY()

public:
	virtual void StartState(UHSMAgentComponent* agent) override;
	virtual void Update(float deltaTime) override;
	virtual void EndState() override;

	virtual const FString GetStateToString() const override;
private:
	void EndAttack();

private:
	FTimerHandle m_ExecuteTimerHandle;
};
