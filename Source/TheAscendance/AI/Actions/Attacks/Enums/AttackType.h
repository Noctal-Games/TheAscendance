// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EAbilityGoal : uint8
{
	DAMAGE UMETA(DisplayName = "Damage"),
	LAST_RESORT_DAMAGE UMETA(DisplayName = "Last Resort Damage"),

	DEBUFF UMETA(DisplayName = "Debuff"),

	BUFF_SELF UMETA(DisplayName = "Buff Self"),
	BUFF_ALLY UMETA(DisplayName = "Buff Ally"),
	HEAL_SELF UMETA(DisplayName = "Heal Self"),
	HEAL_ALLY UMETA(DisplayName = "Heal Ally"),
};
