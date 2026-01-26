// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
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

	for (const TInstancedStruct<FObjectiveTypeData>& objectiveData : m_QuestData->Objectives)
	{
		// Initialize objectives here
	}
}

void UQuest::StartQuest()
{
		for (UBaseObjectiveNode* objective : m_Objectives)
		{
			objective->Activate();
		}
}

void UQuest::UpdateQuest()
{
}

bool UQuest::IsComplete() const
{
	return true;
}