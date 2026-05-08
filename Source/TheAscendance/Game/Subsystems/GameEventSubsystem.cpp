// Fill out your copyright notice in the Description page of Project Settings.


#include "GameEventSubsystem.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Game/Level/Structs/LevelTagData.h"

void UGameEventSubsystem::NotifyItemPickup(const FGameplayTag& itemTag, int amount)
{
	FGameplayTag tag = FGameplayTag::RequestGameplayTag(TEXT("Item"));

	if (itemTag.MatchesTag(tag) == false)
	{
		LOG_WARNING("[GAME EVENT SUBSYSTEM] NotifyItemPickup was triggered but the tag was not an Item tag: %s", *itemTag.ToString());
	}

	OnItemPickup.Broadcast(itemTag, amount);
	OnItemPickupBP.Broadcast(itemTag, amount);
}

void UGameEventSubsystem::NotifyEnemyKilled(const FGameplayTag& enemyTag)
{
	OnEnemyKilled.Broadcast(enemyTag);
	OnEnemyKilledBP.Broadcast(enemyTag);
}

void UGameEventSubsystem::NotifyLocationEnterred(const FGameplayTag& locationTag)
{
	FGameplayTag tag = FGameplayTag::RequestGameplayTag(TEXT("Location"));

	if (locationTag.MatchesTag(tag) == false)
	{
		LOG_WARNING("[GAME EVENT SUBSYSTEM] NotifyLocationEnterred was triggered but the tag was not a Location tag: %s", *locationTag.ToString());
	}
	else
	{
		LOG_ONSCREEN(-1, 5.0f, FColor::Green, "Location Enterred: %s", *locationTag.ToString());
	}

	OnLocationEnterred.Broadcast(locationTag);
	OnLocationEnterredBP.Broadcast(locationTag);
}

void UGameEventSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);

	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UGameEventSubsystem::OnLevelLoaded);
}

void UGameEventSubsystem::Deinitialize()
{
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);

	Super::Deinitialize();
}

void UGameEventSubsystem::OnLevelLoaded(UWorld* world)
{
	if (world == nullptr)
	{
		LOG_WARNING("[GAME EVENT SUBSYSTEM] Level loaded with invalid world");
		return;
	}

	if (const ULevelTagData* data = world->GetWorldSettings()->GetAssetUserData<ULevelTagData>())
	{
		const FGameplayTag& tag = data->LevelTag;
		LOG_INFO("[GAME EVENT SUBSYSTEM] Loaded level: %s", *tag.ToString());
		NotifyLocationEnterred(tag);
	}
	else
	{
		LOG_WARNING("[GAME EVENT SUBSYSTEM] Loaded new level, but the level did not contain a valid LevelTagData asset");
	}
}
