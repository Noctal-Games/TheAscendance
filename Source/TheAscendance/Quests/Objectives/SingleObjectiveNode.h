// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObjectiveNode.h"
#include "TheAscendance/Quests/Structs/ObjectiveData.h"
#include "SingleObjectiveNode.generated.h"

class UBaseObjectiveGoal;

UCLASS()
class THEASCENDANCE_API USingleObjectiveNode : public UBaseObjectiveNode
{
	GENERATED_BODY()
	
public:
	void Init(const FSingleObjectiveData& data);
	void TriggerQuestCompletionCheck();

	virtual void Start() override;
	virtual void Stop() override;

	virtual bool IsComplete() const override;

private:
	UPROPERTY()
	TObjectPtr<UBaseObjectiveGoal> m_Goal = nullptr;

	TSharedPtr<FSingleObjectiveData> m_Data = nullptr;
};
