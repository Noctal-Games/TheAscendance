// Fill out your copyright notice in the Description page of Project Settings.


#include "KillAnyObjectiveGoal.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/Subsystems/QuestManagerSubsystem.h"
#include "TheAscendance/Quests/Structs/ObjectiveData.h"
#include "TheAscendance/Quests/Objectives/SingleObjectiveNode.h"

void UKillAnyObjectiveGoal::Init(const FKillAnyGoal& data)
{
	m_ToKill = data.Amount;
}

void UKillAnyObjectiveGoal::Activate()
{
	if (UQuestManagerSubsystem* questManager = UCoreFunctionLibrary::GetQuestManagerSubsystem())
	{
		m_EnemyKilledHandle = questManager->OnEnemyKilledEvent.AddUObject(this, &UKillAnyObjectiveGoal::OnEnemyKilled);
	}
}

void UKillAnyObjectiveGoal::Deactivate()
{
	if (UQuestManagerSubsystem* questManager = UCoreFunctionLibrary::GetQuestManagerSubsystem())
	{
		questManager->OnEnemyKilledEvent.Remove(m_EnemyKilledHandle);
	}
}

void UKillAnyObjectiveGoal::OnEnemyKilled(const FGameplayTag& enemyTag)
{
	if (IsComplete() == true)
	{
		return;
	}

	m_CurrentAmount += 1;

	if (m_CurrentAmount >= m_ToKill)
	{
		m_IsComplete = true;
		Deactivate();

		if (USingleObjectiveNode* node = Cast<USingleObjectiveNode>(GetOuter()))
		{
			node->TriggerQuestCompletionCheck();
		}
	}
}
