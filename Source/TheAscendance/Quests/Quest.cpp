// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "Structs/QuestData.h"

void UQuest::Init(UQuestData* questData)
{
	if (questData == nullptr)
	{
		LOG_ERROR("Tried to Init quest with invalid QuestData");
		return;
	}

	m_QuestData = questData;
}
