// Fill out your copyright notice in the Description page of Project Settings.


#include "BranchingObjectiveNode.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/Subsystems/QuestManagerSubsystem.h"
#include "TheAscendance/Quests/Goals/BaseObjectiveGoal.h"
#include "OptionalObjectiveNode.h"
#include "TheAscendance/Quests/Quest.h"

void UBranchingObjectiveNode::Init(const FBranchingObjectiveData& data)
{
	if (m_ParentQuest == nullptr)
	{
		LOG_ERROR("[BRANCHING OBJECTIVE NODE] Failed to Init, Parent Quest was invalid");
		return;
	}

	m_Data = MakeShared<FBranchingObjectiveData>(data);

	if (UQuestManagerSubsystem* questManager = UCoreFunctionLibrary::GetQuestManagerSubsystem())
	{
		if (m_Data->ObjectivesPathOne.IsEmpty() == true)
		{
			LOG_ERROR("[BRANCHING OBJECTIVE NODE] Path One is empty");
		}
		else if (m_PathOneStartingObjective = questManager->CreateObjectiveNode(m_ParentQuest.Get(), m_Data->ObjectivesPathOne[0]))
		{
	
		}
		else
		{
			LOG_ERROR("[BRANCHING OBJECTIVE NODE] Failed to create PathOneStartingObjectiveNode for node: %s", *GetName());
		}

		if (m_Data->ObjectivesPathTwo.IsEmpty() == true)
		{
			LOG_ERROR("[BRANCHING OBJECTIVE NODE] Path Two is empty");
		}
		else if (m_PathTwoStartingObjective = questManager->CreateObjectiveNode(m_ParentQuest.Get(), m_Data->ObjectivesPathTwo[0]))
		{

		}
		else
		{
			LOG_ERROR("[BRANCHING OBJECTIVE NODE] Failed to create PathTwoStartingObjectiveNode for node: %s", *GetName());
		}
	}
	else
	{
		LOG_ERROR("[BRANCHING OBJECTIVE NODE] Init failed to get QuestManagerSubsystem");
	}
}

void UBranchingObjectiveNode::Start()
{
	if (m_PathOneStartingObjective != nullptr)
	{
		m_PathOneStartingObjective->OnComplete.BindUObject(this, &UBranchingObjectiveNode::ProcessStartingObjectiveCompletion);
		m_PathOneStartingObjective->Start();
	}
	else
	{
		LOG_ERROR("[BRANCHING OBJECTIVE NODE] PathOneStartingObjectiveNode is invalid");
	}

	if (m_PathTwoStartingObjective != nullptr)
	{
		m_PathTwoStartingObjective->OnComplete.BindUObject(this, &UBranchingObjectiveNode::ProcessStartingObjectiveCompletion);
		m_PathTwoStartingObjective->Start();
	}
	else
	{
		LOG_ERROR("[BRANCHING OBJECTIVE NODE] PathTwoStartingObjectiveNode is invalid");
	}
}

void UBranchingObjectiveNode::Stop()
{
	UBaseObjectiveNode::Stop();

	if (m_PathOneStartingObjective != nullptr)
	{
		m_PathOneStartingObjective->Stop();
	}

	if (m_PathTwoStartingObjective != nullptr)
	{
		m_PathTwoStartingObjective->Stop();
	}

	for (UBaseObjectiveNode* objective : m_Objectives)
	{
		if (objective == nullptr)
		{
			LOG_ERROR("[BRANCHING OBJECTIVE NODE] Contains invalid ObjectiveNode")
			continue;
		}

		objective->Stop();
	}
}

bool UBranchingObjectiveNode::IsComplete() const
{
	if (m_Objectives.IsEmpty() == true)
	{
		return false;
	}

	for (UBaseObjectiveNode* objective : m_Objectives)
	{
		if (objective == nullptr)
		{
			LOG_ERROR("[BRANCHING OBJECTIVE NODE] Contains invalid ObjectiveNode")
			continue;
		}

		if (objective->IsComplete() == false)
		{
			return false;
		}
	}

	return true;
}

void UBranchingObjectiveNode::ProcessStartingObjectiveCompletion()
{
	UQuestManagerSubsystem* questManager = UCoreFunctionLibrary::GetQuestManagerSubsystem();

	if (questManager == nullptr)
	{
		LOG_ERROR("[BRANCHING OBJECTIVE NODE] ProcessStartingObjectiveCompletion failed to get QuestManagerSubsystem");
		return;
	}

	if (m_Objectives.IsEmpty() == false)
	{
		LOG_WARNING("[BRANCHING OBJECTIVE NODE] ProcessStartingObjectiveCompletion called but Objectives already populated, both paths were started at the time time");
		return;
	}

	m_ActiveObjective = nullptr;

	if (m_PathOneStartingObjective != nullptr)
	{
		if (m_PathOneStartingObjective->IsComplete() == true)
		{
			for (int i = 1; i < m_Data->ObjectivesPathOne.Num(); i++)
			{
				const TInstancedStruct<FObjectiveTypeData>& objectiveData = m_Data->ObjectivesPathOne[i];

				if (UBaseObjectiveNode* objectiveNode = questManager->CreateObjectiveNode(m_ParentQuest.Get(), objectiveData))
				{
					objectiveNode->OnComplete.Unbind();
					objectiveNode->OnComplete.BindUObject(this, &UBranchingObjectiveNode::UpdateObjective);

					m_Objectives.Add(objectiveNode);
				}
				else
				{
					LOG_ERROR("[BRANCHING OBJECTIVE NODE] Failed to create ObjectiveNode for node: %s", *GetName());
				}
			}

			if (m_PathTwoStartingObjective != nullptr)
			{
				m_PathTwoStartingObjective->Stop();
				m_PathTwoStartingObjective = nullptr;
			}

			UpdateObjective();
			return;
		}
	}

	if (m_PathTwoStartingObjective != nullptr)
	{
		if (m_PathTwoStartingObjective->IsComplete() == true)
		{
			for (int i = 1; i < m_Data->ObjectivesPathTwo.Num(); i++)
			{
				const TInstancedStruct<FObjectiveTypeData>& objectiveData = m_Data->ObjectivesPathTwo[i];

				if (UBaseObjectiveNode* objectiveNode = questManager->CreateObjectiveNode(m_ParentQuest.Get(), objectiveData))
				{
					objectiveNode->OnComplete.Unbind();
					objectiveNode->OnComplete.BindUObject(this, &UBranchingObjectiveNode::UpdateObjective);

					m_Objectives.Add(objectiveNode);
				}
				else
				{
					LOG_ERROR("[BRANCHING OBJECTIVE NODE] Failed to create ObjectiveNode for node: %s", *GetName());
				}
			}

			if (m_PathOneStartingObjective != nullptr)
			{
				m_PathOneStartingObjective->Stop();
				m_PathOneStartingObjective = nullptr;
			}

			UpdateObjective();
		}
	}
}

void UBranchingObjectiveNode::UpdateObjective()
{
	bool newObjectiveSet = false;

	for (UBaseObjectiveNode* objective : m_Objectives)
	{
		if (objective == nullptr)
		{
			LOG_ERROR("[BRANCHING OBJECTIVE NODE] Contains invalid ObjectiveNode")
			continue;
		}

		//If the objective is optional, always start it. HasStarted check is within Start()
		if (UOptionalObjectiveNode* optionalObjective = Cast<UOptionalObjectiveNode>(objective))
		{
			optionalObjective->Start();
			continue;
		}

		//If a new active objective is set, or objective is already complete, continue
		if (newObjectiveSet == true || objective->IsComplete() == true)
		{
			continue;
		}

		if (objective == m_ActiveObjective)
		{
			newObjectiveSet = true;
			continue;
		}

		m_ActiveObjective = objective;
		m_ActiveObjective->Start();

		newObjectiveSet = true;
	}
}