// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectiveFactory.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Quests/Structs/ObjectiveData.h"
#include "TheAscendance/Quests/Objectives/BaseObjectiveNode.h"
#include "TheAscendance/Quests/Objectives/SingleObjectiveNode.h"

UBaseObjectiveNode* UObjectiveFactory::CreateObjectiveNode(UObject* Outer, const TInstancedStruct<FObjectiveTypeData>& data)
{
	const UScriptStruct* structType = data.GetScriptStruct();

    if (structType == nullptr)
    {
        LOG_ERROR("[OBJECTIVE FACTORY] Tried to create ObjectiveNode with uninitialised data");
        return nullptr;
    }

    if (structType == FSingleObjectiveData::StaticStruct())
    {
        USingleObjectiveNode* objectiveNode = NewObject<USingleObjectiveNode>(Outer);
        objectiveNode->Init(data.Get<FSingleObjectiveData>());
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
