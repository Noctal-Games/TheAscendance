// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatContextEvaluator.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/AI/TAAIController.h"
#include "TheAscendance/Characters/Player/PlayerCharacter.h"
#include "TheAscendance/Characters/Enemies/BaseEnemy.h"

#include "StateTreeExecutionContext.h"

void FCombatContextEvaluator::TreeStart(FStateTreeExecutionContext& context) const
{
    FInstanceDataType& data = context.GetInstanceData(*this);

    data.Target = nullptr;
    data.DistanceToTarget = 0.0f;
}

void FCombatContextEvaluator::Tick(FStateTreeExecutionContext& context, const float deltaTime) const
{
    FInstanceDataType& data = context.GetInstanceData(*this);

    ATAAIController* controller = Cast<ATAAIController>(context.GetOwner());

    if (controller == nullptr)
    {
        LOG_ERROR("[COMBAT CONTEXT EVALUATOR] Controller is invalid");
        return;
    }

    APawn* ownerPawn = controller->GetPawn();

    if (ownerPawn == nullptr)
    {
        LOG_ERROR("[COMBAT CONTEXT EVALUATOR] OwnerPawn is invalid");
        return;
    }

    // Replace with perception system
    AActor* target = UCoreFunctionLibrary::GetPlayerCharacter();
    data.Target = target;

    if (data.Target.IsValid() == false)
    {
        LOG_ERROR("[COMBAT CONTEXT EVALUATOR] Target is invalid");
        return;
    }

    data.DistanceToTarget = FVector::Distance(ownerPawn->GetActorLocation(), target->GetActorLocation());
    data.TargetDetected = true;

    //replace with EnemyData values
    constexpr float preferredRange = 1000.f;
    constexpr float tolerance = 800.f;

    data.TooClose = data.DistanceToTarget < (preferredRange - tolerance);
    data.TooFar = data.DistanceToTarget > (preferredRange + tolerance);

    LOG_ONSCREEN(10, 1.0f, FColor::Yellow, "DETECTED: %s", *FString(data.TargetDetected ? "TRUE" : "FALSE"));
    LOG_ONSCREEN(11, 1.0f, FColor::Yellow, "TOO CLOSE: %s", *FString(data.TooClose ? "TRUE" : "FALSE"));
    LOG_ONSCREEN(12, 1.0f, FColor::Yellow, "TOO FAR: %s", *FString(data.TooFar ? "TRUE" : "FALSE"));
}
