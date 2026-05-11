// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "TheAscendance/Abilities/AbilityFactory.h"
#include "AbilityLoader.generated.h"

class IAbility;
class UAbilityComponent;
class UAbilityData;
class UMeleeData;
struct FSpellTableData;
struct FMeleeTableData;

UCLASS()
class THEASCENDANCE_API UAbilityLoader : public UObject
{
	GENERATED_BODY()
	
public:
	void Init();

	IAbility* CreateAbilityFromTag(const FGameplayTag& abilityTag, UAbilityComponent* abilityOwner);
	IAbility* CreateAbilityFromData(UAbilityData* abilityData, UAbilityComponent* abilityOwner);
	UAbilityData* GetAbilityData(const FGameplayTag& abilityTag);
	FSpellTableData* GetSpellTableDataFromTag(const FGameplayTag& spellTag) const;
	const TArray<TSharedPtr<FSpellTableData>> GetAllSpellTableDataEntries() const;

private:
	USpellData* GetSpellAbilityDataFromTag(const FGameplayTag& abilityTag) const;
	UMeleeData* GetMeleeAbilityDataFromTag(const FGameplayTag& abilityTag) const;

private:
	UPROPERTY()
	TObjectPtr<UDataTable> m_SpellTable = nullptr;
	UPROPERTY()
	TObjectPtr<UDataTable> m_MeleeTable = nullptr;

	TUniquePtr<AbilityFactory> m_AbilityFactory = nullptr;
};
