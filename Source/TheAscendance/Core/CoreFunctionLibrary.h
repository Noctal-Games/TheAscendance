// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "CoreFunctionLibrary.generated.h"

class APlayableGameMode;
class APlayerCharacter;
class ACustomPlayerController;
class UDataHandlerSubsystem;
class UQuestManagerSubsystem;
class UUIManagerSubsystem;
class UGameEventSubsystem;

UCLASS()
class THEASCENDANCE_API UCoreFunctionLibrary : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Core")
	static UWorld* GetGameWorld();

	UFUNCTION(BlueprintPure, Category = "Core")
	static APlayableGameMode* GetPlayableGameMode();

	UFUNCTION(BlueprintPure, Category = "Core")
	static APlayerCharacter* GetPlayerCharacter();
	UFUNCTION(BlueprintPure, Category = "Core")
	static ACustomPlayerController* GetPlayerController();

	UFUNCTION(BlueprintCallable, Category = "Core - Logging")
	static void LogInfo(FString string);
	UFUNCTION(BlueprintCallable, Category = "Core - Logging")
	static void LogWarning(FString string);
	UFUNCTION(BlueprintCallable, Category = "Core - Logging")
	static void LogError(FString string);

	UFUNCTION(BlueprintCallable, Category = "Core - Debug")
	static void DrawDebugLine(const FVector& start, const FVector& end, const FColor colour, const float duration = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Core - Debug")
	static void DrawDebugSphere(const FVector& centre, const float radius, const int32 segments, const FColor colour, const float duration = 1.0f);
	UFUNCTION(BlueprintCallable, Category = "Core - Debug")
	static void DrawDebugBox(const FVector& centre, const FVector& extent, const FRotator& rotation, const FColor colour, const float duration = 1.0f);

	//C++ Only
	static UDataHandlerSubsystem* GetDataHandlerSubsystem();
	static UQuestManagerSubsystem* GetQuestManagerSubsystem();
	static UUIManagerSubsystem* GetUIManagerSubsystem();
	static UGameEventSubsystem* GetGameEventSubsystem();

	template<class UserClass>
	static void SetTimer(FTimerHandle& outHandle, UserClass* inObj, typename FTimerDelegate::TMethodPtr<UserClass> inTimerMethod, float inRate, bool inbLoop = false, float inFirstDelay = -1.0f)
	{
		if (inObj == nullptr)
		{
			LOG_ERROR("[CORE] Tried to SetTimer, but the target object was invalid");
			return;
		}

		if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
		{
			LOG_INFO("[CORE] Setting %f second Timer. Created for Class: %s", inRate, *inObj->GetClass()->GetName());
			world->GetTimerManager().SetTimer(outHandle, inObj, inTimerMethod, inRate, inbLoop, inFirstDelay);
		}
	}

	static void ClearTimerHandle(FTimerHandle& inHandle, const FString handleName = "");
};
