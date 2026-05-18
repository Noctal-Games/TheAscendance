// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PerceptionContext.generated.h"

class ABaseCharacter;

USTRUCT()
struct FTargetPerceptionState
{
	GENERATED_BODY()

	float DetectionValue = 0.0f;

	float TimeSinceSeen = 9999.0f;

	FVector LastKnownLocation = FVector::ZeroVector;

	float LastLOSCheckTime = 0.0f;

	bool HadLOS = false;
};

USTRUCT()
struct FPerceptionContext
{
	GENERATED_BODY()

	UPROPERTY()
	TWeakObjectPtr<ABaseCharacter> Target;
	//Use if ever multiplayer or player has ally NPCs
	//UPROPERTY()
	//TMap<TWeakObjectPtr<ABaseCharacter>, FTargetPerceptionState> KnownTargets;
	UPROPERTY()
	TMap<TWeakObjectPtr<ABaseCharacter>, FTargetPerceptionState> KnownEnemies;
};
