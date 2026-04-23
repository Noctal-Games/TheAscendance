// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemRegistrySubsystem.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/StreamableFunctionLibrary.h"
#include "TheAscendance/Items/Structs/ItemData.h"

#include "Engine/AssetManager.h"

UItemDataAsset* UItemRegistrySubsystem::LoadItemData(const FGameplayTag& itemTag)
{
    LOG_INFO("[ITEM REGISTRY] Loading ItemData asset for tag: %s", *itemTag.ToString());

    TSoftObjectPtr<UItemDataAsset>* item = m_ItemMap.Find(itemTag);

    if (item == nullptr)
    {
        LOG_ERROR("[ITEM REGISTRY] Registry doesn't contain ItemData asset tag: %s", *itemTag.ToString());
        return nullptr;
    }

    UItemDataAsset* loadedItem = UStreamableFunctionLibrary::LoadAsset<UItemDataAsset>(item->ToSoftObjectPath());

    if (loadedItem == nullptr)
    {
        LOG_ERROR("[ITEM REGISTRY] Failed to load ItemData asset for tag: %s", *itemTag.ToString());
        return nullptr;
	}

    LOG_INFO("[ITEM REGISTRY] Successfully loaded ItemData asset for tag: %s", *itemTag.ToString());
	return loadedItem;
}

const UItemDataAsset* UItemRegistrySubsystem::GetItemData(const FGameplayTag& itemTag) const
{
    LOG_INFO("[ITEM REGISTRY] Attempting to Get ItemData asset for tag: %s", *itemTag.ToString());

    if (const TSoftObjectPtr<UItemDataAsset>* item = m_ItemMap.Find(itemTag))
    {
		UItemDataAsset* loadedItem = item->Get();

        if (loadedItem == nullptr)
        {
			LOG_ERROR("[ITEM REGISTRY] Failed to get item data asset for tag: %s. Item was not loaded beforehand", *itemTag.ToString());
        }

        return loadedItem;
    }

    LOG_ERROR("[ITEM REGISTRY] Failed to get item data asset for tag: %s. Item was not found in the Registry", *itemTag.ToString());
    return nullptr;
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

    assetRegistry.GetAssetsByClass(UItemDataAsset::StaticClass()->GetClassPathName(), assetDataList);

    for (const FAssetData& assetData : assetDataList)
    {
        for (const auto& Tag : assetData.TagsAndValues)
        {
            LOG_WARNING("TAG: %s == %s", *Tag.Key.ToString(), *Tag.Value.AsString());
        }

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

        TSoftObjectPtr<UItemDataAsset> softItem(assetPath);
        m_ItemMap.Add(itemTag, softItem);
        LOG_INFO("[ITEM REGISTRY] Registered item: %s -> %s",*itemTag.ToString(), *assetPath.ToString());
    }
}
