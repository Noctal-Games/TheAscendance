// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayableGameMode.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/GameplayTagHelpers.h"
#include "TheAscendance/Game/DataLoaders/ItemLoader.h"
#include "TheAscendance/Game/DataLoaders/SpellLoader.h"
#include "TheAscendance/Game/DataLoaders/EnemyLoader.h"
#include "TheAscendance/Game/DataLoaders/EffectLoader.h"

UBaseEffect* APlayableGameMode::CreateEffectFromTag(const FGameplayTag& effectTag) const
{
	if (m_EffectLoader == nullptr)
	{
		LOG_ERROR("[PLAYABLE GAMEMODE] Invalid EffectLoader");
		return nullptr;
	}

	return m_EffectLoader->CreateEffectFromTag(effectTag);
}

UBaseEffect* APlayableGameMode::CreateEffectFromEffectData(UEffectData* effectData) const
{
	if (m_EffectLoader == nullptr)
	{
		LOG_ERROR("[PLAYABLE GAMEMODE] Invalid EffectLoader");
		return nullptr;
	}

	return m_EffectLoader->CreateEffectFromEffectData(effectData);
}

UEffectData* APlayableGameMode::LoadEffectData(const FGameplayTag& effectTag) const
{
	if (m_EffectLoader == nullptr)
	{
		LOG_ERROR("[PLAYABLE GAMEMODE] Invalid EffectLoader");
		return nullptr;
	}

	return m_EffectLoader->LoadEffectData(effectTag);
}

FItemData* APlayableGameMode::GetItemData(const FGameplayTag& itemTag) const
{
	if (m_ItemLoader == nullptr)
	{
		LOG_ERROR("[PLAYABLE GAMEMODE] Invalid ItemLoader");
		return nullptr;
	}

	return m_ItemLoader->GetItemData(itemTag);
}

FEquippableItemData* APlayableGameMode::GetEquipmentData(const FGameplayTag& itemTag) const
{
	if (m_ItemLoader == nullptr)
	{
		LOG_ERROR("[PLAYABLE GAMEMODE] Invalid ItemLoader");
		return nullptr;
	}

	return nullptr;
}

FWeaponData* APlayableGameMode::GetWeaponData(const FGameplayTag& itemTag) const
{
	if (m_ItemLoader == nullptr)
	{
		LOG_ERROR("[PLAYABLE GAMEMODE] Invalid ItemLoader");
		return nullptr;
	}

	return m_ItemLoader->GetWeaponData(itemTag);
}

const FWeaponTypeData* APlayableGameMode::GetWeaponTypeData(EWeaponType type) const
{
	if (m_ItemLoader == nullptr)
	{
		LOG_ERROR("[PLAYABLE GAMEMODE] Invalid ItemLoader");
		return nullptr;
	}

	return m_ItemLoader->GetWeaponTypeData(type);
}

const FSpellTableData* APlayableGameMode::GetSpellTableData(const FGameplayTag& spellTag) const
{
	if (m_AbilityLoader == nullptr)
	{
		LOG_ERROR("[PLAYABLE GAMEMODE] Invalid AbilityLoader");
		return nullptr;
	}

	return m_AbilityLoader->GetSpellTableDataFromTag(spellTag);
}

IAbility* APlayableGameMode::CreateAbilityFromTag(const FGameplayTag& abilityTag, UAbilityComponent* owner) const
{
	if (m_AbilityLoader == nullptr)
	{
		LOG_ERROR("[PLAYABLE GAMEMODE] Invalid AbilityLoader");
		return nullptr;
	}

	return m_AbilityLoader->CreateAbilityFromTag(abilityTag, owner);
}

ABaseEnemy* APlayableGameMode::CreateEnemyFromID(int enemyID) const
{
	if (m_EnemyLoader == nullptr)
	{
		LOG_ERROR("[PLAYABLE GAMEMODE] Invalid EnemyLoader");
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
		LOG_INFO("[PLAYABLE GAMEMODE] Opened PlayableGameMode BP in Editor");
		return;
	}
#endif

	if (m_ItemLoader = NewObject<UItemLoader>())
	{
		m_ItemLoader->Init();
	}
	else
	{
		LOG_ERROR("[PLAYABLE GAMEMODE] Failed to create ItemLoader");
	}

	if (m_AbilityLoader = NewObject<UAbilityLoader>())
	{
		m_AbilityLoader->Init();
	}
	else
	{
		LOG_ERROR("[PLAYABLE GAMEMODE] Failed to create AbilityLoader");
	}

	if (m_EnemyLoader = NewObject<UEnemyLoader>())
	{
		m_EnemyLoader->Init();
		m_EnemyLoader->SetEnemyDefault(m_EnemyDefault);
	}
	else
	{
		LOG_ERROR("[PLAYABLE GAMEMODE] Failed to create EnemyLoader");
	}

	if (m_EffectLoader = NewObject<UEffectLoader>())
	{
		m_EffectLoader->Init();
	}
	else
	{
		LOG_ERROR("[PLAYABLE GAMEMODE] Failed to create EffectLoader");
	}
}

void APlayableGameMode::StartPlay()
{
	Super::StartPlay();
}

const TArray<TSharedPtr<FSpellTableData>> APlayableGameMode::GetAllSpellTableDataEntries() const
{
	if (m_AbilityLoader == nullptr)
	{
		LOG_ERROR("[PLAYABLE GAMEMODE] Invalid AbilityLoader");
		return TArray<TSharedPtr<FSpellTableData>>();
	}

	return m_AbilityLoader->GetAllSpellTableDataEntries();
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


