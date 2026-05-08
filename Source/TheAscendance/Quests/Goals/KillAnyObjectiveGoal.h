// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObjectiveGoal.h"
#include "GameplayTags.h"
#include "KillAnyObjectiveGoal.generated.h"

struct FKillAnyGoal;

UCLASS()
class THEASCENDANCE_API UKillAnyObjectiveGoal : public UBaseObjectiveGoal
{
	GENERATED_BODY()

public:
	void Init(const FKillAnyGoal& data);

	virtual void Activate() override;
	virtual void Deactivate() override;

protected:
	void OnEnemyKilled(const FGameplayTag& enemyTag);

private:
	int m_ToKill = 0;
	int m_CurrentAmount = 0;

	FDelegateHandle m_EnemyKilledHandle;
};
