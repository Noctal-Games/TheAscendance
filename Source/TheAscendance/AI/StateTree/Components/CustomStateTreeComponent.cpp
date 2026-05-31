// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomStateTreeComponent.h"
#include "TheAscendance/Core/CoreMacros.h"

UCustomStateTreeComponent::UCustomStateTreeComponent(const FObjectInitializer& ObjectInitializer)
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.bStartWithTickEnabled = true;

    SetStartLogicAutomatically(false);
}

void UCustomStateTreeComponent::StartStateTree(UStateTree* stateTree)
{
    //StartLogic();

    if (stateTree == nullptr)
    {
		LOG_ERROR("[CUSTOM STATE TREE COMPONENT] Tried to StartStateTree with invalid StateTree");
        return;
    }

    if (IsRunning() == true)
    {
        Cleanup();
    }

    StateTreeRef.SetStateTree(stateTree);
    RestartLogic();
}

void UCustomStateTreeComponent::StopStateTree()
{
    if (IsRunning())
    {
        Cleanup();
    }

    StopLogic(FString("Stopped Internally"));
    SetStateTree(nullptr);
}

void UCustomStateTreeComponent::ValidateStateTreeReference()
{
    Super::ValidateStateTreeReference();
}

void UCustomStateTreeComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
    Super::TickComponent(deltaTime, tickType, thisTickFunction);

    EStateTreeRunStatus status = GetStateTreeRunStatus();

    UE_LOG(LogTemp, Warning, TEXT(
        "Tick: Registered=%d Active=%d RunStatus=%s HasTree=%d"
    ),
        IsRegistered(),
        IsActive(),
        *UEnum::GetValueAsString(GetStateTreeRunStatus()),
        StateTreeRef.GetStateTree() != nullptr
    );

    LOG_ONSCREEN(9, 1.0f, FColor::Yellow, "STATE STATUS: %s", *UEnum::GetValueAsString(status));
}
