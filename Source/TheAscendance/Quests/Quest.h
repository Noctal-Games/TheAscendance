// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Quest.generated.h"

class UQuestData;

UCLASS()
class THEASCENDANCE_API UQuest : public UObject
{
	GENERATED_BODY()
	
public:
	void Init(UQuestData* questData);

private:
	UPROPERTY()
	TObjectPtr<UQuestData> m_QuestData = nullptr;
};
