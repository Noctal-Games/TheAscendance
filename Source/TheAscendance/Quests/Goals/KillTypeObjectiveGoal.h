// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObjectiveGoal.h"
#include "GameplayTags.h"
#include "KillTypeObjectiveGoal.generated.h"

struct FKillTypeGoal;

UCLASS()
class THEASCENDANCE_API UKillTypeObjectiveGoal : public UBaseObjectiveGoal
{
	GENERATED_BODY()

public:
	void Init(const FKillTypeGoal& data);

	virtual void Activate() override;
	virtual void Deactivate() override;

protected:
	void OnEnemyKilled(const FGameplayTag& enemyTag);

private:
	FGameplayTag m_TargetTag;
	int m_ToKill = 0;
	int m_CurrentAmount = 0;

	FDelegateHandle m_EnemyKilledHandle;
};
