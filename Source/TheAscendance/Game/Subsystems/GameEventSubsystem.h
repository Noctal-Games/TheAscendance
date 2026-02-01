// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameEventSubsystem.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemPickup, int /*ID*/, int /*Amount*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyKilled, int /*ID*/);

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
};
