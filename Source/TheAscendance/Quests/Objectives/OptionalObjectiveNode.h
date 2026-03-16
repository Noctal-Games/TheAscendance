// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObjectiveNode.h"
#include "TheAscendance/Quests/Structs/ObjectiveData.h"
#include "OptionalObjectiveNode.generated.h"

/**
 * 
 */
UCLASS()
class THEASCENDANCE_API UOptionalObjectiveNode : public UBaseObjectiveNode
{
	GENERATED_BODY()
	
public:
	void Init(const FOptionalObjectiveData& data);
	
	virtual void Start() override;
	virtual void Stop() override;

	virtual bool IsComplete() const override;

private:
	UPROPERTY()
	TObjectPtr<UBaseObjectiveNode> m_Objective = nullptr;

	TSharedPtr<FOptionalObjectiveData> m_Data = nullptr;

	bool m_HasStarted = false;
};
