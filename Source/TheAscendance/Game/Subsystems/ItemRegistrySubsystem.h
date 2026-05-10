// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "TheAscendance/Core/StreamableFunctionLibrary.h"
#include "ItemRegistrySubsystem.generated.h"

class UItemData;

UCLASS()
class THEASCENDANCE_API UItemRegistrySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
    UItemData* LoadItemData(const FGameplayTag& itemTag);
    template<typename T>
    T* LoadItemData(const FGameplayTag& itemTag)
    {
        const TSoftObjectPtr<UItemData>* item = m_ItemMap.Find(itemTag);

        if (item == nullptr)
        {
            return nullptr;
        }

        UItemData* base = UStreamableFunctionLibrary::LoadAsset<UItemData>(item->ToSoftObjectPath());
        return Cast<T>(base);
    }

    //Used for async loading
    const TSoftObjectPtr<UItemData>* GetItemRef(const FGameplayTag& itemTag);

    virtual void Initialize(FSubsystemCollectionBase& collection) override;

private:
    void BuildRegistry();

private:
    UPROPERTY()
    TMap<FGameplayTag, TSoftObjectPtr<UItemData>> m_ItemMap;
};
