// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseObjectiveGoal.generated.h"

UCLASS()
class THEASCENDANCE_API UBaseObjectiveGoal : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void Activate();
	virtual void Deactivate();

	virtual bool IsComplete() const;

protected:
	bool m_IsComplete = false;
};
