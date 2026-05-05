// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayTagContainer.h"
#include "TheAscendance/Core/StreamableFunctionLibrary.h"
#include "NPCRegistrySubsystem.generated.h"

class UEnemyData;

UCLASS()
class THEASCENDANCE_API UNPCRegistrySubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

    //Adjust to accomodate non-enemy NPCs if needed in future
public:
    UEnemyData* LoadEnemyData(const FGameplayTag& enemyTag);
    template<typename T>
    T* LoadEnemyData(const FGameplayTag& enemyTag)
    {
        static_assert(TIsDerivedFrom<T, UEnemyData>::Value, "T must derive from UEnemyData");

        const TSoftObjectPtr<UEnemyData>* enemy = m_EnemyMap.Find(enemyTag);

        if (enemy == nullptr)
        {
            return nullptr;
        }

        UEnemyData* base = UStreamableFunctionLibrary::LoadAsset<UEnemyData>(enemy->ToSoftObjectPath());
        return Cast<T>(base);
    }

    //Used for async loading
    const TSoftObjectPtr<UEnemyData>* GetEnemyRef(const FGameplayTag& enemyTag);

    virtual void Initialize(FSubsystemCollectionBase& collection) override;

private:
    void BuildRegistry();

private:
    UPROPERTY()
    TMap<FGameplayTag, TSoftObjectPtr<UEnemyData>> m_EnemyMap;
};
