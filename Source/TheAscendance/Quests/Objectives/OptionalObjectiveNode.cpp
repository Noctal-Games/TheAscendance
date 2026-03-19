// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionalObjectiveNode.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/Subsystems/QuestManagerSubsystem.h"
#include "TheAscendance/Quests/Goals/BaseObjectiveGoal.h"
#include "TheAscendance/Quests/Quest.h"

void UOptionalObjectiveNode::Init(const FOptionalObjectiveData& data)
{
	if (m_ParentQuest == nullptr)
	{
		LOG_ERROR("[MULTI OBJECTIVE NODE] Failed to Init, Parent Quest was invalid");
		return;
	}

	m_Data = MakeShared<FOptionalObjectiveData>(data);

	if (UQuestManagerSubsystem* questManager = UCoreFunctionLibrary::GetQuestManagerSubsystem())
	{
		if (m_Objective = questManager->CreateObjectiveNode(m_ParentQuest.Get(), m_Data->Objective))
		{

		}
		else
		{
			LOG_ERROR("[OPTIONAL OBJECTIVE NODE] Failed to create ObjectiveNode for node: %s", *GetName());
		}
	}
	else
	{
		LOG_ERROR("[OPTIONAL OBJECTIVE NODE] Init failed to get QuestManagerSubsystem");
	}
}

void UOptionalObjectiveNode::Start()
{
	if (m_Objective == nullptr)
	{
		LOG_ERROR("[OPTIONAL OBJECTIVE NODE] Objective is invalid")
		return;
	}

	if (m_HasStarted == false && m_Objective->IsComplete() == false)
	{
		m_Objective->Start();
		m_HasStarted = true;
	}
}

void UOptionalObjectiveNode::Stop()
{
	UBaseObjectiveNode::Stop();

	if (m_Objective == nullptr)
	{
		LOG_ERROR("[OPTIONAL OBJECTIVE NODE] Objective is invalid")
		return;
	}

	m_Objective->Stop();
	m_HasStarted = false;
}

bool UOptionalObjectiveNode::IsComplete() const
{
	//Optional, so should not block quest progress if incomplete (always complete)
	return true;
}
