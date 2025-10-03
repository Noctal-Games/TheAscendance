// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Spells/Enums/SpellModifierType.h"
#include "SpellModifierData.generated.h"

// GENERIC MODIFIERS
USTRUCT(BlueprintType)
struct FGenericSpellModifier
{
	GENERATED_BODY()

	FGenericSpellModifier() : ModifierType(EGenericSpellModifierType::NONE) {};
	FGenericSpellModifier(EGenericSpellModifierType modifierType) : ModifierType(modifierType) {};

	EGenericSpellModifierType ModifierType = EGenericSpellModifierType::NONE;
};

USTRUCT(BlueprintType, meta = (ToolTip = "Applies a specified effect to the spell caster."))
struct FApplyCasterEffectModifier : public FGenericSpellModifier
{
	GENERATED_BODY()

	FApplyCasterEffectModifier() : FGenericSpellModifier(EGenericSpellModifierType::APPLY_CASTER_EFFECT) {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int EffectID = 0;
};

USTRUCT(BlueprintType, meta = (ToolTip = "Applies damage and effects to all valid actors within a specified ranged. This is instant and is separate from spawned AOEs. This is applied on hit with HitScan spells, when the projectile is destroyed with ProjectileSpells, and when casting LocalSpells."))
struct FAreaOfEffectModifier : public FGenericSpellModifier
{
	GENERATED_BODY()

	FAreaOfEffectModifier() : FGenericSpellModifier(EGenericSpellModifierType::AOE) {};
	FAreaOfEffectModifier(const FAreaOfEffectModifier& modifierData) = default;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Range = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "The damage applied by the AOE. Acts as the maximum value if HasDamageFallOff is true."))
	int Damage = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Damage uses the 'Range', 'Damage' and 'DamageMinimum' values to calculate a damage fall-off."))
	bool HasDamageFallOff = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "HasDamageFallOff == true", EditConditionHides))
	int DamageMinimum = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Applies a knockback effect, separate to the KnockbackModifier."))
	bool DoesKnockback = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "DoesKnockback == true", EditConditionHides))
	float KnockbackStrength = 0.0f;
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

USTRUCT(BlueprintType, meta = (ToolTip = "Applies a knockback effect to the hit actor."))
struct FKnockbackSpellModifier : public FRangedSpellModifier
{
	GENERATED_BODY()

	FKnockbackSpellModifier() : FRangedSpellModifier(ERangedSpellModifierType::KNOCKBACK) {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float KnockbackStrength = 0.0f;
};
// RANGED SPELL MODIFIERS

// PROJECTILE SPELL MODIFIERS
USTRUCT(BlueprintType)
struct FProjectileSpellModifier
{
	GENERATED_BODY()

	FProjectileSpellModifier() : ModifierType(EProjectileSpellModifierType::NONE) {};
	FProjectileSpellModifier(EProjectileSpellModifierType modifierType) : ModifierType(modifierType) {};

	EProjectileSpellModifierType ModifierType = EProjectileSpellModifierType::NONE;
};

USTRUCT(BlueprintType, meta = (ToolTip = "A modifier that allows a projectile to pass through hit characters."))
struct FPenetrationSpellModifier : public FProjectileSpellModifier
{
	GENERATED_BODY()

	FPenetrationSpellModifier() : FProjectileSpellModifier(EProjectileSpellModifierType::PENETRATION) {};
	FPenetrationSpellModifier(const FPenetrationSpellModifier& modifierData) = default;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "The maximum amount of characters the projectile can pass through before being destroyed."))
	int PenetrateLimit = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int DamageLossPerHit = 0;
};
// PROJECTILE SPELL MODIFIERS
