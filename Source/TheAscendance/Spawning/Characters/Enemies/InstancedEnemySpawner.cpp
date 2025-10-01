// Fill out your copyright notice in the Description page of Project Settings.


#include "InstancedEnemySpawner.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Characters/Enemies/BaseEnemy.h"

// Sets default values
AInstancedEnemySpawner::AInstancedEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AInstancedEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	if (EnemyID == 0)
	{
		LOG_WARNING("InstancedEnemySpawner has EnemyID of 0");
		return;
	}

	if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
	{
		ABaseEnemy* enemy = gameMode->CreateEnemyFromID(EnemyID);

		if (enemy == nullptr)
		{
			LOG_ERROR("InstancedEnemySpawner failed to load enemy for ID: %i", EnemyID);
			return;
		}

		enemy->SetActorLocation(GetActorLocation());
		enemy->SetActorRotation(GetActorRotation());

		LOG_INFO("InstancedEnemySpawner successfully loaded enemy with ID: %i", EnemyID);
	}
}
