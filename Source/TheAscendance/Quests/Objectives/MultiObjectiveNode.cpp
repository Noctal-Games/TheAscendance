// Fill out your copyright notice in the Description page of Project Settings.
#include "MultiObjectiveNode.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/Subsystems/QuestManagerSubsystem.h"
#include "TheAscendance/Quests/Goals/BaseObjectiveGoal.h"
#include "TheAscendance/Quests/Quest.h"

void UMultiObjectiveNode::Init(const FMultiObjectiveData& data)
{
	if (m_ParentQuest == nullptr)
	{
		LOG_ERROR("[MULTI OBJECTIVE NODE] Failed to Init, Parent Quest was invalid");
		return;
	}

	m_Data = MakeShared<FMultiObjectiveData>(data);

	if (UQuestManagerSubsystem* questManager = UCoreFunctionLibrary::GetQuestManagerSubsystem())
	{
		for (const TInstancedStruct<FObjectiveTypeData>& objectiveData : data.Objectives)
		{
			if (UBaseObjectiveNode* objectiveNode = questManager->CreateObjectiveNode(m_ParentQuest.Get(), objectiveData))
			{
				m_Objectives.Add(objectiveNode);
			}
			else
			{
				LOG_ERROR("[MULTI OBJECTIVE NODE] Failed to create ObjectiveNode for node: %s", *GetName());
			}
		}
	}
	else
	{
		LOG_ERROR("[MULTI OBJECTIVE NODE] Init failed to get QuestManagerSubsystem");
	}
}

void UMultiObjectiveNode::Start()
{
	for (UBaseObjectiveNode* objective : m_Objectives)
	{
		if (objective == nullptr)
		{
			LOG_ERROR("[MULTI OBJECTIVE NODE] Contains invalid ObjectiveNode")
			continue;
		}

		if (objective->IsComplete() == false)
		{
			objective->Start();
		}
	}
}

void UMultiObjectiveNode::Stop()
{
	UBaseObjectiveNode::Stop();

	for (UBaseObjectiveNode* objective : m_Objectives)
	{
		if (objective == nullptr)
		{
			LOG_ERROR("[MULTI OBJECTIVE NODE] Contains invalid ObjectiveNode")
			continue;
		}

		objective->Stop();
	}
}


bool UMultiObjectiveNode::IsComplete() const
{
	for (UBaseObjectiveNode* objective : m_Objectives)
	{
		if (objective == nullptr)
		{
			LOG_ERROR("[MULTI OBJECTIVE NODE] Contains invalid ObjectiveNode")
			continue;
		}

		if (objective->IsComplete() == false)
		{
			return false;
		}
	}

	return true;
}