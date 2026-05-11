// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreFunctionLibrary.h"
#include "TheAscendance/Characters/Player/PlayerCharacter.h"
#include "TheAscendance/Characters/Player/CustomPlayerController.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Game/Subsystems/DataHandlerSubsystem.h"
#include "TheAscendance/Game/Subsystems/QuestManagerSubsystem.h"
#include "TheAscendance/Game/Subsystems/UIManagerSubsystem.h"
#include "TheAscendance/Game/Subsystems/GameEventSubsystem.h"
#include "TheAscendance/AI/Combat/CombatManagerSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

UWorld* UCoreFunctionLibrary::GetGameWorld()
{
	if (GEngine == nullptr)
	{
		//Engine not loaded
		return nullptr;
	}

	for (const FWorldContext& context : GEngine->GetWorldContexts())
	{
		if (context.WorldType == EWorldType::Game || context.WorldType == EWorldType::PIE)
		{
			return context.World();
		}
	}

	LOG_ERROR("[CORE FUNCTION LIBRARY] Unable to find a valid Game World Context");
	return nullptr;
}

APlayableGameMode* UCoreFunctionLibrary::GetPlayableGameMode()
{
	if (UWorld* world = GetGameWorld())
	{
		return world->GetAuthGameMode<APlayableGameMode>();
	}

	LOG_ERROR("[CORE FUNCTION LIBRARY] GameWorld was invalid");
	return nullptr;
}

APlayerCharacter* UCoreFunctionLibrary::GetPlayerCharacter()
{
	if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetGameWorld(), 0)))
	{
		return player;
	}

	LOG_ERROR("[CORE FUNCTION LIBRARY] PlayerCharacter was invalid");
	return nullptr;
}

ACustomPlayerController* UCoreFunctionLibrary::GetPlayerController()
{
	if (ACustomPlayerController* controller = Cast<ACustomPlayerController>(UGameplayStatics::GetPlayerController(GetGameWorld(), 0)))
	{
		return controller;
	}

	LOG_ERROR("[CORE FUNCTION LIBRARY] PlayerController was invalid");
	return nullptr;
}

void UCoreFunctionLibrary::LogInfo(FString string)
{
	LOG_INFO("[CORE FUNCTION LIBRARY - BLUEPRINT] %s", *string);
}

void UCoreFunctionLibrary::LogWarning(FString string)
{
	LOG_WARNING("[CORE FUNCTION LIBRARY - BLUEPRINT] %s", *string);
}

void UCoreFunctionLibrary::LogError(FString string)
{
	LOG_ERROR("[CORE FUNCTION LIBRARY - BLUEPRINT] %s", *string);
}

void UCoreFunctionLibrary::DrawDebugLine(const FVector& start, const FVector& end, const FColor colour, const float duration)
{
#if WITH_EDITOR || UE_BUILD_DEVELOPMENT
	::DrawDebugLine(GetGameWorld(), start, end, colour, false, duration);
#endif
}

void UCoreFunctionLibrary::DrawDebugSphere(const FVector& centre, const float radius, const int32 segments, const FColor colour, const float duration)
{
#if WITH_EDITOR || UE_BUILD_DEVELOPMENT
	::DrawDebugSphere(GetGameWorld(), centre, radius, segments, colour, false, duration);
#endif
}

void UCoreFunctionLibrary::DrawDebugBox(const FVector& centre, const FVector& extent, const FRotator& rotation, const FColor colour, const float duration)
{
#if WITH_EDITOR || UE_BUILD_DEVELOPMENT
	::DrawDebugBox(GetGameWorld(), centre, extent, FQuat(rotation), colour, false, duration);
#endif
}

UDataHandlerSubsystem* UCoreFunctionLibrary::GetDataHandlerSubsystem()
{
	if (UWorld* world = GetGameWorld())
	{
		return world->GetGameInstance()->GetSubsystem<UDataHandlerSubsystem>();
	}

	LOG_ERROR("[CORE FUNCTION LIBRARY] GameWorld was invalid");
	return nullptr;
}

UQuestManagerSubsystem* UCoreFunctionLibrary::GetQuestManagerSubsystem()
{
	if (UWorld* world = GetGameWorld())
	{
		return world->GetGameInstance()->GetSubsystem<UQuestManagerSubsystem>();
	}

	LOG_ERROR("[CORE FUNCTION LIBRARY] GameWorld was invalid");
	return nullptr;
}

UUIManagerSubsystem* UCoreFunctionLibrary::GetUIManagerSubsystem()
{
	if (UWorld* world = GetGameWorld())
	{
		return world->GetGameInstance()->GetSubsystem<UUIManagerSubsystem>();
	}

	LOG_ERROR("[CORE FUNCTION LIBRARY] GameWorld was invalid");
	return nullptr;
}

UGameEventSubsystem* UCoreFunctionLibrary::GetGameEventSubsystem()
{
	if (UWorld* world = GetGameWorld())
	{
		return world->GetGameInstance()->GetSubsystem<UGameEventSubsystem>();
	}

	LOG_ERROR("[CORE FUNCTION LIBRARY] GameWorld was invalid");
	return nullptr;
}

UItemRegistrySubsystem* UCoreFunctionLibrary::GetItemRegistrySubsystem()
{
	if (UWorld* world = GetGameWorld())
	{
		return world->GetGameInstance()->GetSubsystem<UItemRegistrySubsystem>();
	}

	LOG_ERROR("[CORE FUNCTION LIBRARY] GameWorld was invalid");
	return nullptr;
}

UCombatManagerSubsystem* UCoreFunctionLibrary::GetCombatManagerSubsystem()
{
	if (UWorld* world = GetGameWorld())
	{
		return world->GetGameInstance()->GetSubsystem<UCombatManagerSubsystem>();
	}

	LOG_ERROR("[CORE FUNCTION LIBRARY] GameWorld was invalid");
	return nullptr;
}

void UCoreFunctionLibrary::ClearTimerHandle(FTimerHandle& inHandle, const FString handleName)
{
	if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		LOG_INFO("[CORE FUNCTION LIBRARY] Clearing TimerHandle: %s(%s)", *handleName, *inHandle.ToString());
		world->GetTimerManager().ClearTimer(inHandle);
	}
}
