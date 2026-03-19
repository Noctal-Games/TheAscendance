// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObjectiveNode.h"
#include "TheAscendance/Quests/Structs/ObjectiveData.h"
#include "MultiObjectiveNode.generated.h"

UCLASS()
class THEASCENDANCE_API UMultiObjectiveNode : public UBaseObjectiveNode
{
	GENERATED_BODY()
	
public:
	void Init(const FMultiObjectiveData& data);

	virtual void Start() override;
	virtual void Stop() override;

	virtual bool IsComplete() const override;

private:
	UPROPERTY()
	TArray<TObjectPtr<UBaseObjectiveNode>> m_Objectives;

	TSharedPtr<FMultiObjectiveData> m_Data = nullptr;
};
