// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableGameMode.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Game/DataLoaders/ItemLoader.h"
#include "TheAscendance/Game/DataLoaders/SpellLoader.h"
#include "TheAscendance/Game/DataLoaders/EnemyLoader.h"

FItemData* APlayableGameMode::GetItemData(int id)
{
	if (m_ItemLoader == nullptr)
	{
		LOG_ERROR("PlayableGameMode has invalid ItemLoader");
		return nullptr;
	}

	return m_ItemLoader->GetItemData(id);
}

FWeaponData* APlayableGameMode::GetWeaponData(int id)
{
	if (m_ItemLoader == nullptr)
	{
		LOG_ERROR("PlayableGameMode has invalid ItemLoader");
		return nullptr;
	}

	return m_ItemLoader->GetWeaponData(id);
}

const FWeaponTypeData* APlayableGameMode::GetWeaponTypeData(EWeaponType type)
{
	if (m_ItemLoader == nullptr)
	{
		LOG_ERROR("PlayableGameMode has invalid ItemLoader");
		return nullptr;
	}

	return m_ItemLoader->GetWeaponTypeData(type);
}

ISpell* APlayableGameMode::CreateSpellFromID(int spellID, ISpellCaster* spellOwner)
{
	if (m_SpellLoader == nullptr)
	{
		LOG_ERROR("PlayableGameMode has invalid SpellLoader");
		return nullptr;
	}

	return m_SpellLoader->CreateSpellFromID(spellID, spellOwner);
}

ABaseEnemy* APlayableGameMode::CreateEnemyFromID(int enemyID)
{
	if (m_EnemyLoader == nullptr)
	{
		LOG_ERROR("PlayableGameMode has invalid EnemyLoader");
		return nullptr;
	}

	return m_EnemyLoader->CreateEnemyFromID(enemyID);
}

void APlayableGameMode::InitGameState()
{
	Super::InitGameState();

#if WITH_EDITOR
	if (GIsEditor && !GetWorld()->IsGameWorld())
	{
		LOG_INFO("Opened PlayableGameMode BP in Editor");
		return;
	}
#endif

	if (m_ItemLoader = NewObject<UItemLoader>())
	{
		m_ItemLoader->Init();
	}
	else
	{
		LOG_ERROR("PlayableGameMode failed to create ItemLoader");
	}

	if (m_SpellLoader = NewObject<USpellLoader>())
	{
		m_SpellLoader->Init();
	}
	else
	{
		LOG_ERROR("PlayableGameMode failed to create SpellLoader");
	}

	if (m_EnemyLoader = NewObject<UEnemyLoader>())
	{
		m_EnemyLoader->Init();
		m_EnemyLoader->SetEnemyDefault(m_EnemyDefault);
	}
	else
	{
		LOG_ERROR("PlayableGameMode failed to create EnemyLoader");
	}
}

void APlayableGameMode::StartPlay()
{
	Super::StartPlay();
}

void APlayableGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void APlayableGameMode::StartToLeaveMap()
{
	// Save Logic
	Super::StartToLeaveMap();
}


