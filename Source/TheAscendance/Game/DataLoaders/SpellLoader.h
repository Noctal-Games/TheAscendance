// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "TheAscendance/Abilities/AbilityFactory.h"
#include "SpellLoader.generated.h"

class IAbility;
class UAbilityComponent;
struct FSpellTableData;

UCLASS()
class THEASCENDANCE_API UAbilityLoader : public UObject
{
	GENERATED_BODY()
	
public:
	void Init();

	IAbility* CreateAbilityFromTag(const FGameplayTag& spellTag, UAbilityComponent* abilityOwner);
	FSpellTableData* GetSpellTableDataFromTag(const FGameplayTag& spellTag) const;
	const TArray<TSharedPtr<FSpellTableData>> GetAllSpellTableDataEntries() const;

private:
	USpellData* GetSpellAbilityDataFromTag(const FGameplayTag& spellTag) const;

private:
	UPROPERTY()
	TObjectPtr<UDataTable> m_SpellTable = nullptr;

	TUniquePtr<AbilityFactory> m_AbilityFactory = nullptr;
};
