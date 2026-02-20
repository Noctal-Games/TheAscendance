// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObjectiveGoal.h"
#include "GameplayTagContainer.h"
#include "GatherObjectiveGoal.generated.h"

struct FGatherGoal;

UCLASS()
class THEASCENDANCE_API UGatherObjectiveGoal : public UBaseObjectiveGoal
{
	GENERATED_BODY()
	
public:
	void Init(const FGatherGoal& data);

	virtual void Activate() override;
	virtual void Deactivate() override;

protected:
	void OnItemPickup(const FGameplayTag& itemTag, int amount);

private:
	FGameplayTag m_ItemTag;
	int m_ToGather = 0;
	int m_CurrentAmount = 0;

	FDelegateHandle m_ItemPickupHandle;
};
