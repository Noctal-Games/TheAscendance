// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "GameEventSubsystem.generated.h"

//C++ only delegates for internal systems, better for performance. Also executed first.
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemPickup, const FGameplayTag& /*ItemTag*/, int /*Amount*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyKilled, const FGameplayTag& /*EnemyTag*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLocationEnterred, const FGameplayTag& /*LocationTag*/);

//BP assignable delegates for design exposure, less performant but more flexible
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemPickupBP, const FGameplayTag&, itemTag, int, amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyKilledBP, const FGameplayTag&, enemyTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLocationEnterredBP, const FGameplayTag&, locationTag);

UCLASS()
class THEASCENDANCE_API UGameEventSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void NotifyItemPickup(const FGameplayTag& itemTag, int amount);
	void NotifyEnemyKilled(const FGameplayTag& enemyTag);
	void NotifyLocationEnterred(const FGameplayTag& locationTag);

	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

private:
	void OnLevelLoaded(UWorld* world);

public:
	FOnItemPickup OnItemPickup;
	FOnEnemyKilled OnEnemyKilled;
	FOnLocationEnterred OnLocationEnterred;

	UPROPERTY(BlueprintAssignable, Category = "Game Events")
	FOnItemPickupBP OnItemPickupBP;
	UPROPERTY(BlueprintAssignable, Category = "Game Events")
	FOnEnemyKilledBP OnEnemyKilledBP;
	UPROPERTY(BlueprintAssignable, Category = "Game Events")
	FOnLocationEnterredBP OnLocationEnterredBP;
};
