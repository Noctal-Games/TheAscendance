// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractCombatState.h"
#include "RetreatCombatState.generated.h"

/**
 * 
 */
UCLASS()
class THEASCENDANCE_API URetreatCombatState : public UAbstractCombatState
{
	GENERATED_BODY()
	
public:
	virtual void StartState() override;
	virtual void EndState() override;
	virtual void Update(float deltaTime) override;

	virtual const FString GetStateToString() const override;

private:
	void UpdateMovement();
	FVector CalculateRetreatDirection(const FVector& toTarget, float distance, float preferredRange, float aggression) const;

private:
	FVector m_SmoothedDirection = FVector::ZeroVector;

	float m_StateDuration = 0.0f;
	float m_CurrentTime = 0.0f;

	float m_RepathTimer = 0.0f;
	float m_RepathInterval = 0.2f;
};
