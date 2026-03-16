// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseObjectiveNode.generated.h"

class UQuest;
struct FObjectiveTypeData;
	
DECLARE_DELEGATE(FOnObjectiveComplete);

UCLASS()
class THEASCENDANCE_API UBaseObjectiveNode : public UObject
{
	GENERATED_BODY()
	
public:
	void SetParentQuest(UQuest* quest);

	virtual void Start();
	virtual void Stop();

	virtual bool IsComplete() const;

public:
	FOnObjectiveComplete OnComplete;

protected:
	UPROPERTY()
	TWeakObjectPtr<UQuest> m_ParentQuest;
};
