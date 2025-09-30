// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayTagHelpers.h"

FString UGameplayTagHelpers::GetTagSubtype(const FGameplayTag& tag, const FString& prefixToRemove)
{
    FString tagStr = tag.GetTagName().ToString();

    if (prefixToRemove.IsEmpty() == false && tagStr.StartsWith(prefixToRemove))
    {
        tagStr.RightChopInline(prefixToRemove.Len());
    }

    int32 lastDotIndex;

    if (tagStr.FindLastChar('.', lastDotIndex))
    {
        return tagStr.Mid(lastDotIndex + 1);
    }

    return tagStr;
}

FGameplayTag UGameplayTagHelpers::GetBaseTag(const FGameplayTag& tag)
{
    FString tagStr = tag.ToString();

    int32 lastDotIndex;
    if (tagStr.FindLastChar('.', lastDotIndex))
    {
        FString BaseStr = tagStr.Left(lastDotIndex);
        return FGameplayTag::RequestGameplayTag(FName(*BaseStr));
    }

    return tag;
}

bool UGameplayTagHelpers::IsBaseTag(const FGameplayTag& tag)
{
    FString tagStr = tag.GetTagName().ToString();

    return tagStr.Contains(TEXT(".")) == false;
}
