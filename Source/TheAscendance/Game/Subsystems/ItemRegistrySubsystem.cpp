// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemRegistrySubsystem.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Items/Structs/ItemData.h"

#include "Engine/AssetManager.h"

UItemData* UItemRegistrySubsystem::LoadItemData(const FGameplayTag& itemTag)
{
    LOG_INFO("[ITEM REGISTRY] Loading ItemData asset for tag: %s", *itemTag.ToString());

    TSoftObjectPtr<UItemData>* item = m_ItemMap.Find(itemTag);

    if (item == nullptr)
    {
        LOG_ERROR("[ITEM REGISTRY] Registry doesn't contain ItemData asset tag: %s", *itemTag.ToString());
        return nullptr;
    }

    UItemData* loadedItem = UStreamableFunctionLibrary::LoadAsset<UItemData>(item->ToSoftObjectPath());

    if (loadedItem == nullptr)
    {
        LOG_ERROR("[ITEM REGISTRY] Failed to load ItemData asset for tag: %s", *itemTag.ToString());
        return nullptr;
	}

    LOG_INFO("[ITEM REGISTRY] Successfully loaded ItemData asset for tag: %s", *itemTag.ToString());
	return loadedItem;
}

const TSoftObjectPtr<UItemData>* UItemRegistrySubsystem::GetItemRef(const FGameplayTag& itemTag)
{
    if (m_ItemMap.Contains(itemTag) == false)
    {
        LOG_ERROR("[ITEM REGISTRY] Registry doesn't contain ItemData asset tag: %s", *itemTag.ToString());
        return nullptr;
    }

    return &m_ItemMap[itemTag];
}

void UItemRegistrySubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);

	BuildRegistry();
}

void UItemRegistrySubsystem::BuildRegistry()
{
    m_ItemMap.Empty();

    FAssetRegistryModule& assetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    IAssetRegistry& assetRegistry = assetRegistryModule.Get();
    TArray<FAssetData> assetDataList;

    assetRegistry.GetAssetsByClass(UItemData::StaticClass()->GetClassPathName(), assetDataList, true);

    for (const FAssetData& assetData : assetDataList)
    {
        //for (const auto& Tag : assetData.TagsAndValues)
        //{
        //    LOG_WARNING("TAG: %s == %s", *Tag.Key.ToString(), *Tag.Value.AsString());
        //}

        FName itemTagName;
        if (assetData.GetTagValue("ItemTag", itemTagName) == false)
        {
            LOG_ERROR("[ITEM REGISTRY] Missing ItemTag in asset: %s", *assetData.AssetName.ToString());
            continue;
        }

        FGameplayTag itemTag = FGameplayTag::RequestGameplayTag(itemTagName);

        if (itemTag.IsValid() == false)
        {
            LOG_ERROR("[ITEM REGISTRY] ItemTag is invalid for asset: %s", *assetData.AssetName.ToString());
            continue;
        }

        FSoftObjectPath assetPath = assetData.ToSoftObjectPath();

        if (assetPath.IsValid() == false)
        {
            continue;
        }

        TSoftObjectPtr<UItemData> softItem(assetPath);
        m_ItemMap.Add(itemTag, softItem);
        LOG_INFO("[ITEM REGISTRY] Registered item: %s -> %s",*itemTag.ToString(), *assetPath.ToString());
    }
}
