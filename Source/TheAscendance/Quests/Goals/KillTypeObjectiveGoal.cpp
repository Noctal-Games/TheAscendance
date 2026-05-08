// Fill out your copyright notice in the Description page of Project Settings.


#include "KillTypeObjectiveGoal.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/Subsystems/QuestManagerSubsystem.h"
#include "TheAscendance/Quests/Structs/ObjectiveData.h"
#include "TheAscendance/Quests/Objectives/SingleObjectiveNode.h"

void UKillTypeObjectiveGoal::Init(const FKillTypeGoal& data)
{
	m_TargetTag = data.TargetTag;
	m_ToKill = data.Amount;
}

void UKillTypeObjectiveGoal::Activate()
{
	if (UQuestManagerSubsystem* questManager = UCoreFunctionLibrary::GetQuestManagerSubsystem())
	{
		m_EnemyKilledHandle = questManager->OnEnemyKilledEvent.AddUObject(this, &UKillTypeObjectiveGoal::OnEnemyKilled);
	}
}

void UKillTypeObjectiveGoal::Deactivate()
{
	if (UQuestManagerSubsystem* questManager = UCoreFunctionLibrary::GetQuestManagerSubsystem())
	{
		questManager->OnEnemyKilledEvent.Remove(m_EnemyKilledHandle);
	}
}

void UKillTypeObjectiveGoal::OnEnemyKilled(const FGameplayTag& enemyTag)
{
	if (enemyTag != m_TargetTag || IsComplete() == true)
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
