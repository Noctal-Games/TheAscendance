// Fill out your copyright notice in the Description page of Project Settings.
#include "SingleObjectiveNode.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/Subsystems/QuestManagerSubsystem.h"
#include "TheAscendance/Quests/Goals/BaseObjectiveGoal.h"
#include "TheAscendance/Quests/Quest.h"

void USingleObjectiveNode::Init(const FSingleObjectiveData& data)
{
	if (m_ParentQuest == nullptr)
	{
		LOG_ERROR("[SINGLE OBJECTIVE NODE] Failed to Init, Parent Quest was invalid");
		return;
	}

	m_Data = MakeShared<FSingleObjectiveData>(data);

	if (UQuestManagerSubsystem* questManager = UCoreFunctionLibrary::GetQuestManagerSubsystem())
	{
		m_Goal = questManager->CreateObjectiveGoal(this, m_Data->Goal);

		if (m_Goal == nullptr)
		{
			LOG_ERROR("[SINGLE OBJECTIVE NODE] Init failed to create Goal for SingleObjectiveNode");
			return;
		}
	}
	else
	{
		LOG_ERROR("[SINGLE OBJECTIVE NODE] Init failed to get QuestManagerSubsystem");
	}
}

void USingleObjectiveNode::TriggerQuestCompletionCheck()
{
	if (m_ParentQuest == nullptr)
	{
		LOG_ERROR("[SINGLE OBJECTIVE NODE] TriggerQuestCompletionCheck called but ParentQuest is invalid");
		return;
	}

	OnComplete.ExecuteIfBound();
	m_ParentQuest->CheckCompletion();
}

void USingleObjectiveNode::Start()
{
	if(m_Goal == nullptr)
	{
		LOG_ERROR("[SINGLE OBJECTIVE NODE] Start called but Goal is invalid");
		return;
	}

	m_Goal->Activate();
}

void USingleObjectiveNode::Stop()
{
	UBaseObjectiveNode::Stop();

	if(m_Goal == nullptr)
	{
		LOG_ERROR("[SINGLE OBJECTIVE NODE] Stop called but Goal is invalid");
		return;
	}

	m_Goal->Deactivate();
}

bool USingleObjectiveNode::IsComplete() const
{
	if (m_Goal == nullptr)
	{
		LOG_ERROR("[SINGLE OBJECTIVE NODE] IsComplete called but Goal is invalid");
		return false;
	}

    return m_Goal->IsComplete();
}
