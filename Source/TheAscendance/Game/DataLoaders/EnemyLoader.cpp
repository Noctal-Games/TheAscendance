// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyLoader.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/Subsystems/DataHandlerSubsystem.h"
#include "TheAscendance/Characters/Enemies/Structs/EnemyData.h"
#include "TheAscendance/Characters/Enemies/BaseEnemy.h"

void UEnemyLoader::Init()
{
	if (UDataHandlerSubsystem* dataHandler = UCoreFunctionLibrary::GetDataHandlerSubsystem())
	{
		m_EnemyTable = dataHandler->LoadData(EDataGroup::ENEMIES);

		if (m_EnemyTable == nullptr)
		{
			LOG_ERROR("EnemyLoader failed to load Enemy DataTable");
		}
		else
		{
			LOG_INFO("EnemyLoader succeeded to load Enemy DataTable");
		}
	}
}

void UEnemyLoader::SetEnemyDefault(const TSubclassOf<ABaseEnemy>& enemyDefault)
{
	m_EnemyDefault = *enemyDefault;

	if (m_EnemyDefault == nullptr)
	{
		LOG_ERROR("EnemyLoader has no EnemyDefault");
	}
}

ABaseEnemy* UEnemyLoader::CreateEnemyFromID(int enemyID)
{
	if (m_EnemyTable == nullptr)
	{
		LOG_ERROR("EnemyLoader tried to create Enemy without a valid EnemyTable");
		return nullptr;
	}

	if (m_EnemyDefault == nullptr)
	{
		LOG_ERROR("EnemyLoader tried to create Enemy with no EnemyDefault");
		return nullptr;
	}

	static const FString contextString(TEXT("Enemy Context String"));

	TArray<FEnemyTableData*> enemyStructs;
	m_EnemyTable->GetAllRows(contextString, enemyStructs);

	for (const auto data : enemyStructs)
	{
		if (data->EnemyID != enemyID)
		{
			continue;
		}

		if (UWorld* worldContext = UCoreFunctionLibrary::GetGameWorld())
		{
			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			params.bNoFail = true;

			ABaseEnemy* enemy = worldContext->SpawnActor<ABaseEnemy>(m_EnemyDefault, FVector::ZeroVector, FRotator::ZeroRotator, params);

			if (enemy == nullptr)
			{
				return nullptr;
			}

			enemy->Init(data);
			return enemy;                                                                     
		}
	}

	LOG_ERROR("EnemyLoader failed to create Enemy for Enemy ID: %i", enemyID);
	return nullptr;
}
