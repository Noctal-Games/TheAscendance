// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveFactory.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Quests/Structs/ObjectiveData.h"
#include "TheAscendance/Quests/Objectives/BaseObjectiveNode.h"
#include "TheAscendance/Quests/Objectives/SingleObjectiveNode.h"
#include "TheAscendance/Quests/Objectives/MultiObjectiveNode.h"
#include "TheAscendance/Quests/Objectives/OptionalObjectiveNode.h"
#include "TheAscendance/Quests/Objectives/BranchingObjectiveNode.h"
#include "TheAscendance/Quests/Quest.h"

UBaseObjectiveNode* UObjectiveFactory::CreateObjectiveNode(UQuest* parentQuest, const TInstancedStruct<FObjectiveTypeData>& data)
{
	const UScriptStruct* structType = data.GetScriptStruct();

    if (structType == nullptr)
    {
        LOG_ERROR("[OBJECTIVE FACTORY] Tried to create ObjectiveNode with uninitialised data");
        return nullptr;
    }

    if (structType == FSingleObjectiveData::StaticStruct())
    {
        USingleObjectiveNode* objectiveNode = NewObject<USingleObjectiveNode>(parentQuest);
        objectiveNode->SetParentQuest(parentQuest);
        objectiveNode->Init(data.Get<FSingleObjectiveData>());
		return objectiveNode;
    }

    if (structType == FMultiObjectiveData::StaticStruct())
    {
        UMultiObjectiveNode* objectiveNode = NewObject<UMultiObjectiveNode>(parentQuest);
        objectiveNode->SetParentQuest(parentQuest);
        objectiveNode->Init(data.Get<FMultiObjectiveData>());
        return objectiveNode;
    }

    if (structType == FOptionalObjectiveData::StaticStruct())
    {
        UOptionalObjectiveNode* objectiveNode = NewObject<UOptionalObjectiveNode>(parentQuest);
        objectiveNode->SetParentQuest(parentQuest);
        objectiveNode->Init(data.Get<FOptionalObjectiveData>());
        return objectiveNode;
    }

    if (structType == FBranchingObjectiveData::StaticStruct())
    {
        UBranchingObjectiveNode* objectiveNode = NewObject<UBranchingObjectiveNode>(parentQuest);
        objectiveNode->SetParentQuest(parentQuest);
        objectiveNode->Init(data.Get<FBranchingObjectiveData>());
        return objectiveNode;
    }

    if (const FObjectiveTypeData* base = data.GetPtr<FObjectiveTypeData>())
    {
        LOG_ERROR("[OBJECTIVE FACTORY] Failed to create ObjectiveNode for ObjectiveType: %s", *UEnum::GetValueAsString(base->ObjectiveType));
    }
    else
    {
        LOG_ERROR("[OBJECTIVE FACTORY] Failed to create ObjectiveNode (invalid base struct)");
    }

    return nullptr;
}
