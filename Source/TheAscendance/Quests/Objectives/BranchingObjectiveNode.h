// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObjectiveNode.h"
#include "TheAscendance/Quests/Structs/ObjectiveData.h"
#include "BranchingObjectiveNode.generated.h"

UCLASS()
class THEASCENDANCE_API UBranchingObjectiveNode : public UBaseObjectiveNode
{
	GENERATED_BODY()
	
public:
	void Init(const FBranchingObjectiveData& data);

	virtual void Start() override;
	virtual void Stop() override;

	virtual bool IsComplete() const override;

private:
	void ProcessStartingObjectiveCompletion();
	void UpdateObjective();

private:
	UPROPERTY()
	TObjectPtr<UBaseObjectiveNode> m_PathOneStartingObjective = nullptr;
	UPROPERTY()
	TObjectPtr<UBaseObjectiveNode> m_PathTwoStartingObjective = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<UBaseObjectiveNode>> m_Objectives;

	TSharedPtr<FBranchingObjectiveData> m_Data = nullptr;

	UPROPERTY()
	TObjectPtr<UBaseObjectiveNode> m_ActiveObjective = nullptr;
};
