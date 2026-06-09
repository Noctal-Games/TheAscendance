// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryContext_HostileTarget.h"
#include "TheAscendance/Core/CoreMacros.h"

#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Actor.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

void UEnvQueryContext_HostileTarget::ProvideContext(FEnvQueryInstance& queryInstance, FEnvQueryContextData& contextData) const
{
    UObject* ownerObject = queryInstance.Owner.Get();

    AActor* ownerActor = Cast<AActor>(ownerObject);
    AAIController* ownerController = Cast<AAIController>(ownerObject);
    APawn* ownerPawn = Cast<APawn>(ownerObject);


    LOG_WARNING("=== EQS DEBUG CONTEXT ===");

    LOG_WARNING("OwnerObject: %s", *GetNameSafe(ownerObject));
    LOG_WARNING("OwnerActor: %s", *GetNameSafe(ownerActor));
    LOG_WARNING("OwnerController: %s", *GetNameSafe(ownerController));
    LOG_WARNING("OwnerPawn: %s", *GetNameSafe(ownerPawn));

    LOG_WARNING("==========================");
}