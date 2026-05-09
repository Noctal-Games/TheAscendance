// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InstancedEnemySpawner.generated.h"

class AWaypointRoute;
class ABaseEnemy;
class UEnemyData;

UCLASS()
class THEASCENDANCE_API AInstancedEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInstancedEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void SpawnEnemy();

public:
	UPROPERTY(EditInstanceOnly, meta = (Categories = "Character.Enemy"))
	FGameplayTag EnemyTag;
	UPROPERTY(EditInstanceOnly, meta = (DisplayName = "Enemy Default"))
	TSubclassOf<ABaseEnemy> EnemyDefault;

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<AWaypointRoute> PatrolRoute = nullptr;

private:
	UPROPERTY()
	TSoftObjectPtr<UEnemyData> m_EnemyDataAsset = nullptr;
};
