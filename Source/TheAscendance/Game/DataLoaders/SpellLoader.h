// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TheAscendance/Spells/SpellFactory.h"
#include "SpellLoader.generated.h"

class ISpell;
class ISpellCaster;

UCLASS()
class THEASCENDANCE_API USpellLoader : public UObject
{
	GENERATED_BODY()
	
public:
	void Init();

	ISpell* CreateSpellFromID(int spellID, ISpellCaster* spellOwner);

private:
	UPROPERTY()
	TObjectPtr<UDataTable> m_SpellTable = nullptr;

	TUniquePtr<SpellFactory> m_SpellFactory = nullptr;
};
