// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Quest.generated.h"

class UQuestData;
class UBaseObjectiveNode;

UCLASS()
class THEASCENDANCE_API UQuest : public UObject
{
	GENERATED_BODY()
	
public:
	void Init(UQuestData* questData);

	void StartQuest();
	void UpdateQuest();
	bool IsComplete() const;

private:
	UPROPERTY()
	TObjectPtr<UQuestData> m_QuestData = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<UBaseObjectiveNode>> m_Objectives;
};
