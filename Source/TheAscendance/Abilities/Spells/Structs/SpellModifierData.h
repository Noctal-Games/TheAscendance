// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Abilities/Spells/Enums/SpellModifierType.h"
#include "GameplayTagContainer.h"
#include "NiagaraSystem.h"
#include "SpellModifierData.generated.h"

// GENERIC MODIFIERS
USTRUCT()
struct FGenericSpellModifier
{
	GENERATED_BODY()

	FGenericSpellModifier() : ModifierType(EGenericSpellModifierType::NONE) {};
	FGenericSpellModifier(EGenericSpellModifierType modifierType) : ModifierType(modifierType) {};

	EGenericSpellModifierType ModifierType = EGenericSpellModifierType::NONE;
};
// GENERIC MODIFIERS

// RANGED SPELL MODIFIERS
USTRUCT(BlueprintType)
struct FRangedSpellModifier
{
	GENERATED_BODY()

	FRangedSpellModifier() : ModifierType(ERangedSpellModifierType::NONE) {};
	FRangedSpellModifier(ERangedSpellModifierType modifierType) : ModifierType(modifierType) {};

	ERangedSpellModifierType ModifierType = ERangedSpellModifierType::NONE;
};
// RANGED SPELL MODIFIERS
