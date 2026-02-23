// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "StructUtils/InstancedStruct.h"
#include "QuestManagerSubsystem.generated.h"

class UQuestData;
class UQuest;
class UObjectiveFactory;
class UObjectiveGoalFactory;
class UBaseObjectiveNode;
class UBaseObjectiveGoal;
struct FObjectiveTypeData;
struct FObjectiveGoalData;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnQuestItemPickup, const FGameplayTag& /*itemTag*/, int /*amount*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestEnemyKilled, int /*id*/);

UCLASS()
class THEASCENDANCE_API UQuestManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	UQuest* StartQuest(UQuestData* data);
	UFUNCTION(BlueprintCallable)
	void CompleteQuest(UQuest* quest);

	UFUNCTION(BlueprintCallable)
	UQuest* GetQuestByTag(FGameplayTag tag) const;

	UBaseObjectiveNode* CreateObjectiveNode(UQuest* outer, const TInstancedStruct<FObjectiveTypeData>& data);
	UBaseObjectiveGoal* CreateObjectiveGoal(UBaseObjectiveNode* outer, const TInstancedStruct<FObjectiveGoalData>& data);

	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

protected:
	UQuest* CreateQuest(UQuestData* data);

private:
	void HandleItemPickup(const FGameplayTag& itemTag, int amount);
	void HandleEnemyKilled(int id);

protected:
	friend class UGatherObjectiveGoal;
	friend class UKillAnyObjectiveGoal;
	friend class UKillTypeObjectiveGoal;

	FOnQuestItemPickup OnItemPickupEvent;
	FOnQuestEnemyKilled OnEnemyKilledEvent;

private:
	FDelegateHandle OnItemPickupGameEventHandle;
	FDelegateHandle OnEnemyKilledGameEventHandle;

	TArray<TObjectPtr<UQuest>> m_ActiveQuests;

	UPROPERTY()
	TObjectPtr<UObjectiveFactory> m_ObjectiveFactory = nullptr;
	UPROPERTY()
	TObjectPtr<UObjectiveGoalFactory> m_ObjectiveGoalFactory = nullptr;
};
