// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "ObjectiveGoalFactory.generated.h"

class UBaseObjectiveGoal;
struct FObjectiveGoalData;

UCLASS()
class THEASCENDANCE_API UObjectiveGoalFactory : public UObject
{
	GENERATED_BODY()
	
public:
	UBaseObjectiveGoal* CreateGoal(UObject* Outer, const TInstancedStruct<FObjectiveGoalData>& data);
};
