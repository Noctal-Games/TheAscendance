// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InstancedStruct.h"
#include "GameplayTagContainer.h"
#include "TheAscendance/Abilities/Structs/AbilityModifierData.h"
#include "TheAscendance/Abilities/Enums/AbilityType.h"
#include "AbilityData.generated.h"

class UAnimMontage;
class USpellData;

//Make abstract
UCLASS(BlueprintType)
class THEASCENDANCE_API UAbilityData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Cooldown = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> AbilityAnimation = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FAbilityModifier>> AbilityModifiers;

	EAbilityType AbilityType = EAbilityType::NONE;
};