// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class IAbility;
class UAbilityComponent;
class UAbilityData;
class ISpell;
class ISpellCaster;
class USpellData;

class THEASCENDANCE_API AbilityFactory
{
public:
	AbilityFactory() = default;
	~AbilityFactory() = default;

	IAbility* CreateAbility(UAbilityData* abilityData, UAbilityComponent* ownerComponent);

private:
	void ProcessSpellAbility(const USpellData& spellData, const IAbility& ability);
};
