// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCRegistrySubsystem.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Characters/Enemies/Structs/EnemyData.h"

#include "Engine/AssetManager.h"

UEnemyData* UNPCRegistrySubsystem::LoadEnemyData(const FGameplayTag& enemyTag)
{
    LOG_INFO("[NPC REGISTRY] Loading EnemyData asset for tag: %s", *enemyTag.ToString());

    TSoftObjectPtr<UEnemyData>* enemy = m_EnemyMap.Find(enemyTag);

    if (enemy == nullptr)
    {
        LOG_ERROR("[NPC REGISTRY] Registry doesn't contain EnemyData asset tag: %s", *enemyTag.ToString());
        return nullptr;
    }

    UEnemyData* loadedEnemy = UStreamableFunctionLibrary::LoadAsset<UEnemyData>(enemy->ToSoftObjectPath());

    if (loadedEnemy == nullptr)
    {
        LOG_ERROR("[NPC REGISTRY] Failed to load EnemyData asset for tag: %s", *enemyTag.ToString());
        return nullptr;
    }

    LOG_INFO("[NPC REGISTRY] Successfully loaded EnemyData asset for tag: %s", *enemyTag.ToString());
    return loadedEnemy;
}

const TSoftObjectPtr<UEnemyData>* UNPCRegistrySubsystem::GetEnemyRef(const FGameplayTag& enemyTag)
{
    if (m_EnemyMap.Contains(enemyTag) == false)
    {
        LOG_ERROR("[NPC REGISTRY] Registry doesn't contain ItemData asset tag: %s", *enemyTag.ToString());
        return nullptr;
    }

    return &m_EnemyMap[enemyTag];
}

void UNPCRegistrySubsystem::Initialize(FSubsystemCollectionBase& collection)
{
    Super::Initialize(collection);

    BuildRegistry();
}

void UNPCRegistrySubsystem::BuildRegistry()
{
    m_EnemyMap.Empty();

    FAssetRegistryModule& assetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& assetRegistry = assetRegistryModule.Get();
    TArray<FAssetData> assetDataList;

    assetRegistry.GetAssetsByClass(UEnemyData::StaticClass()->GetClassPathName(), assetDataList, true);

    for (const FAssetData& assetData : assetDataList)
    {
        FName enemyTagName;
        if (assetData.GetTagValue("EnemyTag", enemyTagName) == false)
        {
            LOG_ERROR("[NPC REGISTRY] Missing EnemyTag in asset: %s", *assetData.AssetName.ToString());
            continue;
        }

        FGameplayTag enemyTag = FGameplayTag::RequestGameplayTag(enemyTagName);

        if (enemyTag.IsValid() == false)
        {
            LOG_ERROR("[ENEMY REGISTRY] EnemyTag is invalid for asset: %s", *assetData.AssetName.ToString());
            continue;
        }

        FSoftObjectPath assetPath = assetData.ToSoftObjectPath();

        if (assetPath.IsValid() == false)
        {
            continue;
        }

        TSoftObjectPtr<UEnemyData> softEnemy(assetPath);
        m_EnemyMap.Add(enemyTag, softEnemy);
        LOG_INFO("[NPC REGISTRY] Registered enemy: %s -> %s", *enemyTag.ToString(), *assetPath.ToString());
    }
}
