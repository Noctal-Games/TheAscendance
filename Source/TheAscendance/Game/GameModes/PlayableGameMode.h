// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TheAscendance/Items/Enums/WeaponType.h"
#include "TheAscendance/Core/GameplayTagHelpers.h"
#include "PlayableGameMode.generated.h"

class UEffectLoader;
class UEffectData;
class UBaseEffect;
class UEnemyLoader;
class ABaseEnemy;
class USpellLoader;
class ISpell;
class ISpellCaster;
class UItemLoader;
struct FItemData;
struct FWeaponData;
struct FWeaponTypeData;

UCLASS(Blueprintable)
class THEASCENDANCE_API APlayableGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UBaseEffect* CreateEffectFromTag(const FGameplayTag& effectTag);
	UBaseEffect* CreateEffectFromEffectData(UEffectData* effectData);
	UEffectData* LoadEffectData(const FGameplayTag& effectTag);

	FItemData* GetItemData(int id);
	FWeaponData* GetWeaponData(int itemID);
	const FWeaponTypeData* GetWeaponTypeData(EWeaponType type);

	ISpell* CreateSpellFromID(int spellID, ISpellCaster* spellOwner);

	ABaseEnemy* CreateEnemyFromID(int enemyID);

	virtual void InitGameState() override;

	virtual void StartPlay() override;
	virtual void StartToLeaveMap() override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UItemLoader> m_ItemLoader = nullptr;
	UPROPERTY()
	TObjectPtr<USpellLoader> m_SpellLoader = nullptr;
	UPROPERTY()
	TObjectPtr<UEnemyLoader> m_EnemyLoader = nullptr;
	UPROPERTY()
	TObjectPtr<UEffectLoader> m_EffectLoader = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Enemy Default"))
	TSubclassOf<ABaseEnemy> m_EnemyDefault;
};
