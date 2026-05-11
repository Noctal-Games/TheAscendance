// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Abilities/Enums/AbilityModifierType.h"
#include "AbilityModifierData.generated.h"

class UNiagaraSystem;

// GENERIC MODIFIERS
USTRUCT()
struct FAbilityModifier
{
	GENERATED_BODY()

	FAbilityModifier() : ModifierType(EAbilityModifierType::NONE) {};
	FAbilityModifier(EAbilityModifierType modifierType) : ModifierType(modifierType) {};

	EAbilityModifierType ModifierType = EAbilityModifierType::NONE;
};

USTRUCT(BlueprintType)
struct FChargedAbilityModifier : public FAbilityModifier
{
	GENERATED_BODY()

	FChargedAbilityModifier() : FAbilityModifier(EAbilityModifierType::CHARGED) {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> ChargingAnimation = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool DoesChargeCompleteForceAbility = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsAnimationLooped = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "IsAnimationLooped", EditConditionHides = true))
	float MaxChargeTime = 0.0f;
};

USTRUCT(BlueprintType, meta = (ToolTip = "Applies damage and effects to all valid actors within a specified ranged. This is instant and is separate from spawned AOEs. This is applied on hit with HitScan spells, when the projectile is destroyed with ProjectileSpells, and when casting LocalSpells."))
struct FAreaOfEffectModifier : public FAbilityModifier
{
	GENERATED_BODY()

	FAreaOfEffectModifier() : FAbilityModifier(EAbilityModifierType::AOE) {};
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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UNiagaraSystem> AOEHitNiagara = nullptr;
};

USTRUCT(BlueprintType, meta = (ToolTip = "Applies a specified effect to hit targets. Appliable Effects include Base Effects and Chain Effects"))
struct FApplyEffectModifier : public FAbilityModifier
{
	GENERATED_BODY()

	FApplyEffectModifier() : FAbilityModifier(EAbilityModifierType::APPLY_EFFECT) {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag EffectTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "If the effect has a chance to land, and isn't guaranteed"))
	bool HasRNG = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "HasRNG == true", EditConditionHides, ClampMin = 0.0f, ClampMax = 100.0f))
	float ChanceToApply = 0.0f;
};

USTRUCT(BlueprintType, meta = (ToolTip = "Applies a knockback effect to direct hits on actors. This is separate from the knockback applied by AOE"))
struct FKnockbackAbilityModifier : public FAbilityModifier
{
	GENERATED_BODY()

	FKnockbackAbilityModifier() : FAbilityModifier(EAbilityModifierType::KNOCKBACK) {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float KnockbackStrength = 0.0f;
};

USTRUCT(BlueprintType, meta = (ToolTip = "Applies an effect to the ability user."))
struct FApplyUserEffectAbilityModifier : public FAbilityModifier
{
	GENERATED_BODY()

	FApplyUserEffectAbilityModifier() : FAbilityModifier(EAbilityModifierType::APPLY_USER_EFFECT) {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag EffectTag;
};

USTRUCT(BlueprintType, meta = (ToolTip = "Spawns a specified effect at the hit location. Spawnable Effects include AOE."))
struct FSpawnEffectAbilityModifier : public FAbilityModifier
{
	GENERATED_BODY()

	FSpawnEffectAbilityModifier() : FAbilityModifier(EAbilityModifierType::SPAWN_EFFECT) {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag EffectTag;
};