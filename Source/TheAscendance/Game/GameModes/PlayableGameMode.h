// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TheAscendance/Items/Enums/WeaponType.h"
#include "GameplayTagContainer.h"
#include "PlayableGameMode.generated.h"

class UEffectLoader;
class UEffectData;
class UBaseEffect;
class UEnemyLoader;
class ABaseEnemy;
class UAbilityLoader;
class UItemLoader;
class IAbility;
class UAbilityComponent;
struct FItemData;
struct FEquippableItemData;
struct FWeaponData;
struct FWeaponTypeData;
struct FSpellTableData;

UCLASS(Blueprintable)
class THEASCENDANCE_API APlayableGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UBaseEffect* CreateEffectFromTag(const FGameplayTag& effectTag) const;
	UBaseEffect* CreateEffectFromEffectData(UEffectData* effectData) const;
	UEffectData* LoadEffectData(const FGameplayTag& effectTag) const;

	FItemData* GetItemData(const FGameplayTag& itemTag) const;
	FEquippableItemData* GetEquipmentData(const FGameplayTag& itemTag) const;
	FWeaponData* GetWeaponData(const FGameplayTag& itemTag) const;
	const FWeaponTypeData* GetWeaponTypeData(EWeaponType type) const;

	const FSpellTableData* GetSpellTableData(const FGameplayTag& spellTag) const;

	IAbility* CreateAbilityFromTag(const FGameplayTag& abilityTag, UAbilityComponent* owner) const;

	UFUNCTION(BlueprintCallable)
	ABaseEnemy* CreateEnemyFromID(int enemyID) const;

	virtual void InitGameState() override;

	virtual void StartPlay() override;
	virtual void StartToLeaveMap() override;

protected:
	friend class UGrimoire;

	const TArray<TSharedPtr<FSpellTableData>> GetAllSpellTableDataEntries() const;

	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UItemLoader> m_ItemLoader = nullptr;
	UPROPERTY()
	TObjectPtr<UAbilityLoader> m_AbilityLoader = nullptr;
	UPROPERTY()
	TObjectPtr<UEnemyLoader> m_EnemyLoader = nullptr;
	UPROPERTY()
	TObjectPtr<UEffectLoader> m_EffectLoader = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Enemy Default"))
	TSubclassOf<ABaseEnemy> m_EnemyDefault;
};
