// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractState.h"
#include "IdleState.generated.h"

class UHSMAgentComponent;
class UNavigationSystemV1;

UCLASS()
class THEASCENDANCE_API UIdleState : public UAbstractState
{
	GENERATED_BODY()
	
public:
	virtual void StartState(UHSMAgentComponent* agent) override;
	virtual void Update(float deltaTime) override;
	virtual void EndState() override;

private:
	TWeakObjectPtr<UNavigationSystemV1> m_NavigationSystem = nullptr;
};
