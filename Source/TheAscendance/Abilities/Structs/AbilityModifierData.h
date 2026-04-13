// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Abilities/Enums/AbilityModifierType.h"
#include "AbilityModifierData.generated.h"

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
	bool IsAnimationLooped = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "IsAnimationLooped", EditConditionHides = true))
	float MaxChargeTime = 0.0f;
};