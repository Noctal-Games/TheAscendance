// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Quests/Structs/ObjectiveData.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "QuestData.generated.h"

UCLASS(BlueprintType)
class THEASCENDANCE_API UQuestData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Quest"))
	FGameplayTag QuestTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FObjectiveTypeData>> Objectives;
};

USTRUCT(BlueprintType)
struct FQuestlineTableData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Quest"))
	FGameplayTag QuestlineTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSoftObjectPtr<UQuestData>> QuestData;
};