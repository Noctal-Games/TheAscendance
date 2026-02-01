// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameEventSubsystem.generated.h"

//C++ only delegates for internal systems, better for performance. Also executed first.
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemPickup, int, int);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyKilled, int);

//BP assignable delegates for design exposure, less performant but more flexible
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemPickupBP, int, ID, int, Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyKilledBP, int, ID);

UCLASS()
class THEASCENDANCE_API UGameEventSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void NotifyItemPickup(int id, int amount);
	void NotifyEnemyKilled(int id);

	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

public:
	FOnItemPickup OnItemPickup;
	FOnEnemyKilled OnEnemyKilled;

	UPROPERTY(BlueprintAssignable, Category = "Game Events")
	FOnItemPickupBP OnItemPickupBP;
	UPROPERTY(BlueprintAssignable, Category = "Game Events")
	FOnEnemyKilledBP OnEnemyKilledBP;
};
