#include "ItemData.h"
// Fill out your copyright notice in the Description page of Project Settings.

void UItemDataAsset::GetAssetRegistryTags(TArray<FAssetRegistryTag>& outTags) const
{
    Super::GetAssetRegistryTags(outTags);

    outTags.Add(FAssetRegistryTag(
        TEXT("ItemTag"),
        ItemTag.ToString(),
        FAssetRegistryTag::TT_Alphabetical
    ));
}
