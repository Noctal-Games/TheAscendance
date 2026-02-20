// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEventSubsystem.h"
#include "TheAscendance/Core/CoreMacros.h"

void UGameEventSubsystem::NotifyItemPickup(const FGameplayTag& itemTag, int amount)
{
	OnItemPickup.Broadcast(itemTag, amount);
	OnItemPickupBP.Broadcast(itemTag, amount);
}

void UGameEventSubsystem::NotifyEnemyKilled(int id)
{
	OnEnemyKilled.Broadcast(id);
	OnEnemyKilledBP.Broadcast(id);
}

void UGameEventSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);
}

void UGameEventSubsystem::Deinitialize()
{
	Super::Deinitialize();
}
