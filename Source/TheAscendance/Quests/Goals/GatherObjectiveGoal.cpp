// Fill out your copyright notice in the Description page of Project Settings.
#include "GatherObjectiveGoal.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/Subsystems/QuestManagerSubsystem.h"
#include "TheAscendance/Quests/Structs/ObjectiveData.h"
#include "TheAscendance/Quests/Objectives/SingleObjectiveNode.h"

void UGatherObjectiveGoal::Init(const FGatherGoal& data)
{
	m_ItemTag = data.ItemTag;
	m_ToGather = data.Amount;
}

void UGatherObjectiveGoal::Activate()
{
	if (UQuestManagerSubsystem* questManager = UCoreFunctionLibrary::GetQuestManagerSubsystem())
	{
		questManager->OnItemPickupEvent.AddUObject(this, &UGatherObjectiveGoal::OnItemPickup);
	}
}

void UGatherObjectiveGoal::Deactivate()
{
	if (UQuestManagerSubsystem* questManager = UCoreFunctionLibrary::GetQuestManagerSubsystem())
	{
		questManager->OnItemPickupEvent.RemoveAll(this);
	}
}

void UGatherObjectiveGoal::OnItemPickup(const FGameplayTag& itemTag, int amount)
{
	if (itemTag != m_ItemTag || IsComplete() == true)
	{
		return;
	}

	m_CurrentAmount += amount;

	if (m_CurrentAmount >= m_ToGather)
	{
		m_IsComplete = true;
		Deactivate();

		if (USingleObjectiveNode* node = Cast<USingleObjectiveNode>(GetOuter()))
		{
			LOG_ONSCREEN(-1, 5.0f, FColor::Green, "GOAL COMPLETE");
			node->TriggerQuestCompletionCheck();
		}
	}
}
