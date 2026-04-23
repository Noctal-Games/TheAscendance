// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "ItemRegistrySubsystem.generated.h"

class UItemDataAsset;

UCLASS()
class THEASCENDANCE_API UItemRegistrySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
    UItemDataAsset* LoadItemData(const FGameplayTag& itemTag);
    const UItemDataAsset* GetItemData(const FGameplayTag& itemTag) const;

    virtual void Initialize(FSubsystemCollectionBase& collection) override;

private:
    void BuildRegistry();

private:
    UPROPERTY()
    TMap<FGameplayTag, TSoftObjectPtr<UItemDataAsset>> m_ItemMap;
};
