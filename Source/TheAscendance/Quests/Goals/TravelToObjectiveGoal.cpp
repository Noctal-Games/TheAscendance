// Fill out your copyright notice in the Description page of Project Settings.


#include "TravelToObjectiveGoal.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/Subsystems/QuestManagerSubsystem.h"
#include "TheAscendance/Quests/Structs/ObjectiveData.h"
#include "TheAscendance/Quests/Objectives/SingleObjectiveNode.h"
#include "TheAscendance/Game/Subsystems/GameEventSubsystem.h"

void UTravelToObjectiveGoal::Init(const FTravelToGoal& data)
{
	m_LocationTag = data.LocationTag;
}

void UTravelToObjectiveGoal::Activate()
{
	if (UQuestManagerSubsystem* questManager = UCoreFunctionLibrary::GetQuestManagerSubsystem())
	{
		m_TravelToHandle = questManager->OnLocationEnterredEvent.AddUObject(this, &UTravelToObjectiveGoal::OnLocationEnterred);
	}
}

void UTravelToObjectiveGoal::Deactivate()
{
	if (UQuestManagerSubsystem* questManager = UCoreFunctionLibrary::GetQuestManagerSubsystem())
	{
		questManager->OnLocationEnterredEvent.Remove(m_TravelToHandle);
	}
}

void UTravelToObjectiveGoal::OnLocationEnterred(const FGameplayTag& locationTag)
{
	if (locationTag != m_LocationTag || IsComplete() == true)
	{
		return;
	}

	m_IsComplete = true;
	Deactivate();

	if (USingleObjectiveNode* node = Cast<USingleObjectiveNode>(GetOuter()))
	{
		LOG_ONSCREEN(-1, 5.0f, FColor::Green, "GOAL COMPLETE");
		node->TriggerQuestCompletionCheck();
	}
}
