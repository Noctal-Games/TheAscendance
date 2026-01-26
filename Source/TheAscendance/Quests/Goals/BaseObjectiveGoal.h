// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseObjectiveGoal.generated.h"

struct FObjectiveTypeData;

UCLASS()
class THEASCENDANCE_API UBaseObjectiveGoal : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void Init(const FObjectiveTypeData& data);
	virtual void Activate();
	virtual bool IsCompleted() const;

protected:
	bool m_IsComplete = false;
};
