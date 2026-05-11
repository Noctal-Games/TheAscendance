// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Actors/Projectile/Enums/ProjectileModifierType.h"
#include "GameplayTagContainer.h"
#include "ProjectileModifierData.generated.h"

class UNiagaraSystem;

USTRUCT()
struct FProjectileModifier
{
	GENERATED_BODY()

	FProjectileModifier() : ModifierType(EProjectileModifierType::NONE) {};
	FProjectileModifier(EProjectileModifierType modifierType) : ModifierType(modifierType) {};

	EProjectileModifierType ModifierType = EProjectileModifierType::NONE;
};

USTRUCT(BlueprintType, meta = (ToolTip = "A modifier that allows a projectile to pass through hit characters."))
struct FPenetrationProjectileModifier : public FProjectileModifier
{
	GENERATED_BODY()

	FPenetrationProjectileModifier() : FProjectileModifier(EProjectileModifierType::PENETRATION) {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "The maximum amount of characters the projectile can pass through before being destroyed."))
	int PenetrateLimit = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int DamageLossPerHit = 0;
};

USTRUCT(BlueprintType)
struct FTrailProjectileModifier : public FProjectileModifier
{
	GENERATED_BODY()

	FTrailProjectileModifier() : FProjectileModifier(EProjectileModifierType::TRAIL) {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag TrailEffectTag;
};