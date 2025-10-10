// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CoreFunctionLibrary.generated.h"

class APlayableGameMode;
class APlayerCharacter;
class ATAPlayerController;
class UDataHandlerSubsystem;

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
	static ATAPlayerController* GetPlayerController();

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
	static void RequestAsyncLoad(const FSoftObjectPath& targetToStream, TFunction<void()> delegate = nullptr);
	static void RequestAsyncLoad(const TArray<FSoftObjectPath>& targetsToStream, TFunction<void()> delegate = nullptr);
};
