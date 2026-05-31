// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StateTreeAIComponent.h"
#include "CustomStateTreeComponent.generated.h"

class UStateTree;

UCLASS()
class THEASCENDANCE_API UCustomStateTreeComponent : public UStateTreeAIComponent
{
	GENERATED_BODY()
	
public:
	UCustomStateTreeComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    void StartStateTree(UStateTree* stateTree);
    void StopStateTree();

	virtual void ValidateStateTreeReference() override;
	virtual void TickComponent(float deltaTime, enum ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;
};
