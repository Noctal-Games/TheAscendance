// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Quests/Enums/ObjectiveType.h"
#include "StructUtils/InstancedStruct.h"
#include "GameplayTagContainer.h"
#include "ObjectiveData.generated.h"

// OBJECTIVE GOALS
USTRUCT()
struct FObjectiveGoalData
{
	GENERATED_BODY()

	FObjectiveGoalData() {};
	FObjectiveGoalData(EObjectiveGoalType objectiveGoal) : ObjectiveGoal(objectiveGoal) {};

	EObjectiveGoalType ObjectiveGoal = EObjectiveGoalType::NONE;
};

USTRUCT(BlueprintType, meta = (ToolTip = "Gather a specified amount of an item"))
struct FGatherGoal : public FObjectiveGoalData
{
	GENERATED_BODY()

	FGatherGoal() : FObjectiveGoalData(EObjectiveGoalType::GATHER) {};

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Item"))
	FGameplayTag ItemTag;
	UPROPERTY(EditDefaultsOnly)
	int Amount = 0;
};
USTRUCT(BlueprintType, meta = (ToolTip = "Kill specified amount of enemies of any type"))
struct FKillAnyGoal : public FObjectiveGoalData
{
	GENERATED_BODY()

	FKillAnyGoal() : FObjectiveGoalData(EObjectiveGoalType::KILL_ANY) {};

	UPROPERTY(EditDefaultsOnly)
	int Amount = 0;
};
USTRUCT(BlueprintType, meta = (ToolTip = "Kill specified amount of a specific enemy type"))
struct FKillTypeGoal : public FObjectiveGoalData
{
	GENERATED_BODY()

	FKillTypeGoal() : FObjectiveGoalData(EObjectiveGoalType::KILL_TYPE) {};

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag TargetTag;
	UPROPERTY(EditDefaultsOnly)
	int Amount = 0;
};
USTRUCT(BlueprintType, meta = (ToolTip = "Travel to a specified location"))
struct FTravelToGoal : public FObjectiveGoalData
{
	GENERATED_BODY()

	FTravelToGoal() : FObjectiveGoalData(EObjectiveGoalType::TRAVEL_TO) {};

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Location"))
	FGameplayTag LocationTag;
};

// OBJECTIVE TYPES
USTRUCT()
struct FObjectiveTypeData
{
	GENERATED_BODY()

	FObjectiveTypeData() : ObjectiveType(EObjectiveType::NONE) {};
	FObjectiveTypeData(EObjectiveType objectiveType) : ObjectiveType(objectiveType) {};

	EObjectiveType ObjectiveType = EObjectiveType::NONE;
};

USTRUCT(BlueprintType, meta = (ToolTip = "Complete objective"))
struct FSingleObjectiveData : public FObjectiveTypeData
{
	GENERATED_BODY()

	FSingleObjectiveData() : FObjectiveTypeData(EObjectiveType::SINGLE) {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ExcludeBaseStruct))
	TInstancedStruct<FObjectiveGoalData> Goal;
};

USTRUCT(BlueprintType, meta = (ToolTip = "Complete objective path A or B"))
struct FBranchingObjectiveData : public FObjectiveTypeData
{
	GENERATED_BODY()

	FBranchingObjectiveData() : FObjectiveTypeData(EObjectiveType::BRANCHING) {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FObjectiveTypeData>> ObjectivesPathOne;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FObjectiveTypeData>> ObjectivesPathTwo;
};

USTRUCT(BlueprintType, meta = (ToolTip = "Complete all objectives"))
struct FMultiObjectiveData : public FObjectiveTypeData
{
	GENERATED_BODY()

	FMultiObjectiveData() : FObjectiveTypeData(EObjectiveType::MULTI) {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FObjectiveTypeData>> Objectives;
};

USTRUCT(BlueprintType, meta = (ToolTip = "Optional completion of objective"))
struct FOptionalObjectiveData : public FObjectiveTypeData
{
	GENERATED_BODY()

	FOptionalObjectiveData() : FObjectiveTypeData(EObjectiveType::OPTIONAL) {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ExcludeBaseStruct))
	TInstancedStruct<FObjectiveTypeData> Objective;
};
