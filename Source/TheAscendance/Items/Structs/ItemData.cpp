#include "ItemData.h"
// Fill out your copyright notice in the Description page of Project Settings.

void UItemData::GetAssetRegistryTags(FAssetRegistryTagsContext context) const
{
    context.AddTag(FAssetRegistryTag(
        TEXT("ItemTag"),
        ItemTag.ToString(),
        FAssetRegistryTag::TT_Hidden
    ));

    Super::GetAssetRegistryTags(context);
}
