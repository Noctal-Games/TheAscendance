// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TheAscendance/Spells/SpellFactory.h"
#include "GameplayTagContainer.h"
#include "SpellLoader.generated.h"

class ISpell;
class ISpellCaster;
struct FSpellTableData;

UCLASS()
class THEASCENDANCE_API USpellLoader : public UObject
{
	GENERATED_BODY()
	
public:
	void Init();

	ISpell* CreateSpellFromTag(const FGameplayTag& spellTag, ISpellCaster* spellOwner) const;
	FSpellTableData* GetSpellTableDataFromTag(const FGameplayTag& spellTag) const;
	const TArray<TSharedPtr<FSpellTableData>> GetAllSpellTableDataEntries() const;

private:
	UPROPERTY()
	TObjectPtr<UDataTable> m_SpellTable = nullptr;

	TUniquePtr<SpellFactory> m_SpellFactory = nullptr;
};
