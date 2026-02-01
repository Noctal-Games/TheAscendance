// Fill out your copyright notice in the Description page of Project Settings.
#include "GatherObjectiveGoal.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/Subsystems/QuestManagerSubsystem.h"
#include "TheAscendance/Quests/Structs/ObjectiveData.h"
#include "TheAscendance/Quests/Objectives/SingleObjectiveNode.h"

void UGatherObjectiveGoal::Init(const FGatherGoal& data)
{
	m_ItemID = data.ItemID;
	m_ToGather = data.Amount;
}

void UGatherObjectiveGoal::Activate()
{
	if (UQuestManagerSubsystem* questManager = UCoreFunctionLibrary::GetQuestManagerSubsystem())
	{
		m_ItemPickupHandle = questManager->OnItemPickupEvent.AddUObject(this, &UGatherObjectiveGoal::OnItemPickup);
	}
}

void UGatherObjectiveGoal::Deactivate()
{
	if (UQuestManagerSubsystem* questManager = UCoreFunctionLibrary::GetQuestManagerSubsystem())
	{
		questManager->OnItemPickupEvent.Remove(m_ItemPickupHandle);
	}
}

void UGatherObjectiveGoal::OnItemPickup(int id, int amount)
{
	if (id != m_ItemID || IsComplete() == true)
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
			node->TriggerQuestCompletionCheck();
		}
	}
}
