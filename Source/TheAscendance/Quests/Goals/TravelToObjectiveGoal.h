// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObjectiveGoal.h"
#include "GameplayTagContainer.h"
#include "TravelToObjectiveGoal.generated.h"

struct FTravelToGoal;

UCLASS()
class THEASCENDANCE_API UTravelToObjectiveGoal : public UBaseObjectiveGoal
{
	GENERATED_BODY()
	
public:
	void Init(const FTravelToGoal& data);

	virtual void Activate() override;
	virtual void Deactivate() override;

protected:
	void OnLocationEnterred(const FGameplayTag& locationTag);

private:
	FGameplayTag m_LocationTag;

	FDelegateHandle m_TravelToHandle;
};
