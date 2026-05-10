// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractCombatState.h"
#include "ApproachCombatState.generated.h"

/**
 * 
 */
UCLASS()
class THEASCENDANCE_API UApproachCombatState : public UAbstractCombatState
{
	GENERATED_BODY()
	
public:
	virtual void StartState() override;
	virtual void EndState() override;
	virtual void Update(float deltaTime) override;

	virtual const FString GetStateToString() const override;
};
