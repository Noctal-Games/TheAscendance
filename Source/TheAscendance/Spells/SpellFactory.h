// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class ISpell;
class ISpellCaster;
class USpellData;

class THEASCENDANCE_API SpellFactory
{
public:
	SpellFactory() = default;
	~SpellFactory() = default;

	ISpell* CreateSpell(USpellData* spellData, ISpellCaster* spellOwner);
};
