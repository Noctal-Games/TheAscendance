// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEventSubsystem.h"
#include "TheAscendance/Core/CoreMacros.h"

void UGameEventSubsystem::NotifyItemPickup(int id, int amount)
{
	OnItemPickup.Broadcast(id, amount);
	OnItemPickupBP.Broadcast(id, amount);
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
