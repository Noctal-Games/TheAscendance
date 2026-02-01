// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/Subsystems/QuestManagerSubsystem.h"
#include "Structs/QuestData.h"
#include "Structs/ObjectiveData.h"
#include "Objectives/BaseObjectiveNode.h"

void UQuest::Init(UQuestData* questData)
{
	if (questData == nullptr)
	{
		LOG_ERROR("Tried to Init quest with invalid QuestData");
		return;
	}

	m_QuestData = questData;
	m_Objectives.Empty();

	if(UQuestManagerSubsystem* questManager = UCoreFunctionLibrary::GetQuestManagerSubsystem())
	{
		for (const TInstancedStruct<FObjectiveTypeData>& objectiveData : m_QuestData->Objectives)
		{
			if (UBaseObjectiveNode* objectiveNode = questManager->CreateObjectiveNode(this, objectiveData))
			{
				objectiveNode->SetParentQuest(this);
				m_Objectives.Add(objectiveNode);
			}
			else
			{
				LOG_ERROR("[QUEST] Failed to create ObjectiveNode for Quest: %s", *GetName());
			}
		}
	}
	else
	{
		LOG_ERROR("[QUEST] Tried to Init quest but could not find QuestManagerSubsystem");
	}
}

void UQuest::StartQuest()
{
	for (UBaseObjectiveNode* objective : m_Objectives)
	{
		if (objective == nullptr)
		{
			LOG_ERROR("[QUEST] Quest contains invalid ObjectiveNode")
			continue;
		}

		objective->Start();
	}
}

void UQuest::CheckCompletion()
{
	if (IsComplete() == false)
	{
		return;
	}

	if (UQuestManagerSubsystem* questManager = UCoreFunctionLibrary::GetQuestManagerSubsystem())
	{
		questManager->CompleteQuest(this);
	}
	else
	{
		LOG_ERROR("[QUEST] CheckCompletion called but could not find QuestManagerSubsystem");
	}
}

void UQuest::MarkAsComplete()
{
	for (UBaseObjectiveNode* objective : m_Objectives)
	{
		if (objective == nullptr)
		{
			LOG_ERROR("[QUEST] Quest contains invalid ObjectiveNode")
			continue;
		}

		objective->Stop();
	}
}

bool UQuest::IsComplete() const
{
	for (UBaseObjectiveNode* objective : m_Objectives)
	{
		if (objective == nullptr)
		{
			LOG_ERROR("[QUEST] Quest contains invalid ObjectiveNode")
			continue;
		}

		if(objective->IsComplete() == false)
		{
			return false;
		}
	}

	return true;
}

FGameplayTag UQuest::GetQuestTag() const
{
	if (m_QuestData != nullptr)
	{
		return m_QuestData->QuestTag;
	}

	return FGameplayTag();
}
