// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/AI/Actions/Attacks/Enums/AttackType.h"
#include "AttackData.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct FAttackData
{
	GENERATED_BODY()

	FAttackData() : AttackType(EMeleeAttackType::QUICK_ATTACK) {};
	FAttackData(EMeleeAttackType attackType) : AttackType(attackType) {};

	EMeleeAttackType AttackType;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UAnimMontage> TelegraphMontage = nullptr;
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UAnimMontage> AttackMontage = nullptr;
};

USTRUCT(BlueprintType)
struct FQuickAttackData : public FAttackData
{
	GENERATED_BODY()

	FQuickAttackData() : FAttackData(EMeleeAttackType::QUICK_ATTACK) {};
};

USTRUCT(BlueprintType)
struct FHeavyAttackData : public FAttackData
{
	GENERATED_BODY()

	FHeavyAttackData() : FAttackData(EMeleeAttackType::HEAVY_ATTACK) {};
};

USTRUCT(BlueprintType)
struct FLungeAttackData : public FAttackData
{
	GENERATED_BODY()

	FLungeAttackData() : FAttackData(EMeleeAttackType::LUNGE_ATTACK) {};
};