// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreFunctionLibrary.h"
#include "TheAscendance/Characters/Player/PlayerCharacter.h"
#include "TheAscendance/Characters/Player/TAPlayerController.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Game/Subsystems/DataHandlerSubsystem.h"
#include "TheAscendance/Core/CoreMacros.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"

UWorld* UCoreFunctionLibrary::GetGameWorld()
{
	if (UGameViewportClient* viewport = GEngine->GameViewport)
	{
		return viewport->GetWorld();
	}

	LOG_ERROR("GameViewport was invalid");
	return nullptr;
}

APlayableGameMode* UCoreFunctionLibrary::GetPlayableGameMode()
{
	if (UWorld* world = GetGameWorld())
	{
		return world->GetAuthGameMode<APlayableGameMode>();
	}

	LOG_ERROR("GameWorld was invalid");
	return nullptr;
}

APlayerCharacter* UCoreFunctionLibrary::GetPlayerCharacter()
{
	if (APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetGameWorld(), 0)))
	{
		return player;
	}

	LOG_ERROR("PlayerCharacter was invalid");
	return nullptr;
}

ATAPlayerController* UCoreFunctionLibrary::GetPlayerController()
{
	if (ATAPlayerController* controller = Cast<ATAPlayerController>(UGameplayStatics::GetPlayerController(GetGameWorld(), 0)))
	{
		return controller;
	}

	LOG_ERROR("PlayerController was invalid");
	return nullptr;
}

void UCoreFunctionLibrary::LogInfo(FString string)
{
	LOG_INFO("%s", *string);
}

void UCoreFunctionLibrary::LogWarning(FString string)
{
	LOG_WARNING("%s", *string);
}

void UCoreFunctionLibrary::LogError(FString string)
{
	LOG_ERROR("%s", *string);
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

	LOG_ERROR("GameWorld was invalid");
	return nullptr;
}

void UCoreFunctionLibrary::RequestAsyncLoad(const FSoftObjectPath& targetToStream, TFunction<void()> delegate)
{
	LOG_INFO("Requesting ASync Load for: %s", *targetToStream.ToString());

	UAssetManager::GetStreamableManager().RequestAsyncLoad(targetToStream, FStreamableDelegate::CreateLambda([delegate = MoveTemp(delegate), targetToStream]()
		{
			UObject* loadedObject = targetToStream.ResolveObject();

			if (loadedObject != nullptr)
			{
				LOG_INFO("Successful ASync Load for: %s", *targetToStream.ToString());
			}
			else
			{
				LOG_ERROR("Failed ASync Load for: %s", *targetToStream.ToString());
			}

			if (delegate.IsSet())
			{
				delegate();
			}
		}
	));
}

void UCoreFunctionLibrary::RequestAsyncLoad(const TArray<FSoftObjectPath>& targetsToStream, TFunction<void()> delegate)
{
	if (targetsToStream.Num() == 0)
	{
		LOG_WARNING("Tried to RequestASyncLoad for multiple items, but the array was empty");

		if (delegate.IsSet())
		{
			delegate();
		}

		return;
	}
	else if (targetsToStream.Num() == 1)
	{
		LOG_WARNING("Requesting ASync Load for multiple items, but the array only contains one item. Consider using the single load alternative.");
	}

	for (const FSoftObjectPath& path : targetsToStream)
	{
		LOG_INFO("Requesting ASync Load for: %s", *path.ToString());
	}

	UAssetManager::GetStreamableManager().RequestAsyncLoad(targetsToStream, FStreamableDelegate::CreateLambda([delegate = MoveTemp(delegate), targetsToStream]()
		{
			for (const FSoftObjectPath& path : targetsToStream)
			{
				UObject* loadedObject = path.ResolveObject();

				if (loadedObject != nullptr)
				{
					LOG_INFO("Successful ASync Load for: %s", *path.ToString());
					continue;
				}
				
				LOG_ERROR("Failed ASync Load for: %s", *path.ToString());
			}

			if (delegate.IsSet())
			{
				delegate();
			}
		}
	));
}
