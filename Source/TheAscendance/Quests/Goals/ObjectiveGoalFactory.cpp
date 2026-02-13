// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveGoalFactory.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Quests/Structs/ObjectiveData.h"
#include "TheAscendance/Quests/Goals/BaseObjectiveGoal.h"
#include "TheAscendance/Quests/Goals/GatherObjectiveGoal.h"

UBaseObjectiveGoal* UObjectiveGoalFactory::CreateGoal(UObject* Outer, const TInstancedStruct<FObjectiveGoalData>& data)
{
	const UScriptStruct* structType = data.GetScriptStruct();

	if (structType == nullptr)
	{
		LOG_ERROR("[GOAL FACTORY] Tried to create ObjectiveGoal with uninitialised data");
		return nullptr;
	}

	if (structType == FGatherGoal::StaticStruct())
	{
		UGatherObjectiveGoal* goal = NewObject<UGatherObjectiveGoal>(Outer);
		goal->Init(data.Get<FGatherGoal>());
		return goal;
	}

	if (const FObjectiveGoalData* base = data.GetPtr<FObjectiveGoalData>())
	{
		LOG_ERROR("[GOAL FACTORY] Failed to create Goal for GoalType: %s", *UEnum::GetValueAsString(base->ObjectiveGoal));
	}
	else
	{
		LOG_ERROR("[GOAL FACTORY] Failed to create ObjectiveGoal (invalid base struct)");
	}

	return nullptr;
}
