// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "GameplayTagHelpers.generated.h"

UCLASS()
class THEASCENDANCE_API UGameplayTagHelpers : public UObject
{
	GENERATED_BODY()
	
public:
    //Gets the final element in the tag. So Effect.Burn.AOE will return AOE
    UFUNCTION(BlueprintPure, Category = "GameplayTags")
    static FString GetTagSubtype(const FGameplayTag& tag, const FString& prefixToRemove = "");

    //Returns Base Tag. So Effect.Burn.AOE will return Effect.Burn
    UFUNCTION(BlueprintPure, Category = "GameplayTags")
    static FGameplayTag GetBaseTag(const FGameplayTag& tag);

    //True if tag has no children
    UFUNCTION(BlueprintPure, Category = "GameplayTags")
    static bool IsBaseTag(const FGameplayTag& tag);
};
