// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractCombatState.h"
#include "StrafeCombatState.generated.h"

/**
 * 
 */
UCLASS()
class THEASCENDANCE_API UStrafeCombatState : public UAbstractCombatState
{
	GENERATED_BODY()

public:
	virtual void StartState() override;
	virtual void EndState() override;
	virtual void Update(float deltaTime) override;

	virtual const FString GetStateToString() const override;
private:
	void UpdateMovement();
	void ChooseDirection();
	bool ShouldFlipDirection() const;

private:
	float m_StrafeDuration = 0.0f;
	float m_CurrentTime = 0.0f;
	float m_StrafeDirection = 1.0f;
	float m_TargetDistance = 0.0f;

	bool m_IsCircling = true;
};
