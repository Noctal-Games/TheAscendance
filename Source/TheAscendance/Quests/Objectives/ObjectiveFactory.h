// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "StructUtils/InstancedStruct.h"
#include "ObjectiveFactory.generated.h"

class UBaseObjectiveNode;
class UQuest;
struct FObjectiveTypeData;

UCLASS()
class THEASCENDANCE_API UObjectiveFactory : public UObject
{
	GENERATED_BODY()
	
public:
	UBaseObjectiveNode* CreateObjectiveNode(UQuest* parentQuest, const TInstancedStruct<FObjectiveTypeData>& data);
};
