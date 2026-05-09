// Fill out your copyright notice in the Description page of Project Settings.


#include "InstancedEnemySpawner.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Characters/Enemies/BaseEnemy.h"
#include "TheAscendance/Characters/Enemies/Structs/EnemyData.h"
#include "TheAscendance/Game/Subsystems/NPCRegistrySubsystem.h"

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
	
	if (EnemyTag.IsValid() == false)
	{
		LOG_WARNING("[INSTANCED ENEMY SPAWNER] EnemyTag is invalid");
		return;
	}

	UWorld* world = UCoreFunctionLibrary::GetGameWorld();

	if (world == nullptr)
	{
		LOG_ERROR("[INSTANCED ENEMY SPAWNER] Failed to get World reference");
		return;
	}

	if (UNPCRegistrySubsystem* registry = world->GetGameInstance()->GetSubsystem<UNPCRegistrySubsystem>())
	{
		if (const TSoftObjectPtr<UEnemyData>* enemyRef = registry->GetEnemyRef(EnemyTag))
		{
			m_EnemyDataAsset = *enemyRef;

			TWeakObjectPtr<AInstancedEnemySpawner> weakThis(this);

			UStreamableFunctionLibrary::RequestAsyncLoad(m_EnemyDataAsset.ToSoftObjectPath(), [weakThis]()
				{
					if (weakThis.IsValid())
					{
						weakThis->SpawnEnemy();
					}
				}
			);
		}
		else
		{
			LOG_ERROR("[INSTANCED ENEMY SPAWNER] Failed to find EnemyData asset for tag: %s", *EnemyTag.ToString());
			return;
		}

	}
}

void AInstancedEnemySpawner::SpawnEnemy()
{
	if(m_EnemyDataAsset.IsValid() == false)
	{
		LOG_ERROR("[INSTANCED ENEMY SPAWNER] Tried to spawn enemy with invalid EnemyData asset: %s", *EnemyTag.ToString());
		return;
	}

	if (EnemyDefault == nullptr)
	{
		LOG_ERROR("[INSTANCED ENEMY SPAWNER] EnemyDefault is invalid");
		return;
	}

	UWorld* world = UCoreFunctionLibrary::GetGameWorld();

	if (world == nullptr)
	{
		LOG_ERROR("[INSTANCED ENEMY SPAWNER] Failed to get World reference");
		return;
	}

	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	params.bNoFail = true;

	ABaseEnemy* enemy = world->SpawnActor<ABaseEnemy>(EnemyDefault, FVector::ZeroVector, FRotator::ZeroRotator, params);

	if (enemy == nullptr)
	{
		return;
	}

	enemy->Init(m_EnemyDataAsset.Get());  

	enemy->SetActorLocation(GetActorLocation());
	enemy->SetActorRotation(GetActorRotation());

	if (PatrolRoute != nullptr)
	{
		enemy->SetWaypointRoute(PatrolRoute);
	}

	LOG_INFO("[INSTANCED ENEMY SPAWNER] Successfully loaded enemy: %s", *EnemyTag.ToString());
}
