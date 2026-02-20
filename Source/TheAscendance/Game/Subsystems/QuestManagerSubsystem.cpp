// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestManagerSubsystem.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Quests/Quest.h"
#include "TheAscendance/Quests/Structs/QuestData.h"
#include "TheAscendance/Quests/Objectives/ObjectiveFactory.h"
#include "TheAscendance/Quests/Goals/ObjectiveGoalFactory.h"
#include "TheAscendance/Game/Subsystems/GameEventSubsystem.h"
#include "TheAscendance/Quests/Objectives/BaseObjectiveNode.h"

UQuest* UQuestManagerSubsystem::StartQuest(UQuestData* data)
{
	if (data == nullptr)
	{
		LOG_ERROR("[QUEST MANAGER] Attempted to StartQuest with invalid quest data");
		return nullptr;
	}

	if (GetQuestByTag(data->QuestTag))
	{
		LOG_WARNING("[QUEST MANAGER] Attempted to StartQuest but quest is already active: %s", *data->QuestTag.ToString());
		return nullptr;
	}

	if (UQuest* quest = CreateQuest(data))
	{
		LOG_ONSCREEN(-1, 1.0f, FColor::Blue, "QUEST CREATED");

		m_ActiveQuests.Add(quest);
		quest->StartQuest();
		return quest;
	}

	return nullptr;
}

void UQuestManagerSubsystem::CompleteQuest(UQuest* quest)
{
	if(quest == nullptr)
	{
		LOG_ERROR("[QUEST MANAGER] Attempted to CompleteQuest with invalid quest");
		return;
	}

	LOG_ONSCREEN(-1, 1.0f, FColor::Green, "COMPLETED QUEST");

	m_ActiveQuests.Remove(quest);
	quest->MarkAsComplete();
}

UQuest* UQuestManagerSubsystem::GetQuestByTag(FGameplayTag tag) const
{
	for (UQuest* quest : m_ActiveQuests)
	{
		if (quest && quest->GetQuestTag() == tag)
		{
			return quest;
		}
	}

	LOG_WARNING("[QUEST MANAGER] GetQuestByTag could not find active quest with tag: %s", *tag.ToString());
	return nullptr;
}

UBaseObjectiveNode* UQuestManagerSubsystem::CreateObjectiveNode(UQuest* outer, const TInstancedStruct<FObjectiveTypeData>& data)
{
	if (m_ObjectiveFactory == nullptr)
	{
		LOG_ERROR("[QUEST MANAGER] CreateObjectiveNode called with invalid ObjectiveFactory");
		return nullptr;
	}

	return m_ObjectiveFactory->CreateObjectiveNode(outer, data);
}

UBaseObjectiveGoal* UQuestManagerSubsystem::CreateObjectiveGoal(UBaseObjectiveNode* outer, const TInstancedStruct<FObjectiveGoalData>& data)
{
	if (m_ObjectiveGoalFactory == nullptr)
	{
		LOG_ERROR("[QUEST MANAGER] CreateObjectiveGoal called with invalid ObjectiveGoalFactory");
		return nullptr;
	}

	return m_ObjectiveGoalFactory->CreateGoal(outer, data);
}

void UQuestManagerSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);

	m_ObjectiveFactory = NewObject<UObjectiveFactory>(this);
	m_ObjectiveGoalFactory = NewObject<UObjectiveGoalFactory>(this);

	if (UGameEventSubsystem* EventSystem = GetGameInstance()->GetSubsystem<UGameEventSubsystem>())
	{
		OnItemPickupGameEventHandle = EventSystem->OnItemPickup.AddUObject(this, &UQuestManagerSubsystem::HandleItemPickup);
		OnEnemyKilledGameEventHandle = EventSystem->OnEnemyKilled.AddUObject(this, &UQuestManagerSubsystem::HandleEnemyKilled);
	}
}

void UQuestManagerSubsystem::Deinitialize()
{
	if (UGameEventSubsystem* EventSystem = GetGameInstance()->GetSubsystem<UGameEventSubsystem>())
	{
		EventSystem->OnItemPickup.Remove(OnItemPickupGameEventHandle);
		EventSystem->OnEnemyKilled.Remove(OnEnemyKilledGameEventHandle);
	}

	Super::Deinitialize();
}

UQuest* UQuestManagerSubsystem::CreateQuest(UQuestData* data)
{
	if (UQuest* quest = NewObject<UQuest>(this))
	{
		quest->Init(data);
		return quest;
	}

	LOG_ERROR("[QUEST MANAGER] CreateQuest failed to create quest object for quest with tag: %s", *data->QuestTag.ToString());
	return nullptr;
}

void UQuestManagerSubsystem::HandleItemPickup(const FGameplayTag& itemTag, int amount)
{
	OnItemPickupEvent.Broadcast(itemTag, amount);
}

void UQuestManagerSubsystem::HandleEnemyKilled(int id)
{
	OnEnemyKilledEvent.Broadcast(id);
}
