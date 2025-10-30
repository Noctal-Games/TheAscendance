// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbstractState.generated.h"

class UHSMAgentComponent;

UCLASS(Abstract)
class THEASCENDANCE_API UAbstractState : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void StartState(UHSMAgentComponent* agent);
	virtual void Update(float deltaTime) PURE_VIRTUAL(UAbstractState::Update, );
	virtual void EndState();

protected:
	TWeakObjectPtr<UHSMAgentComponent> m_Agent = nullptr;
};
