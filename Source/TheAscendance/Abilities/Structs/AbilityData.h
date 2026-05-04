// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InstancedStruct.h"
#include "GameplayTagContainer.h"
#include "TheAscendance/Abilities/Structs/AbilityModifierData.h"
#include "TheAscendance/Abilities/Enums/AbilityType.h"
#include "TheAscendance/Characters/Enums/CharacterStat.h"
#include "AbilityData.generated.h"

class UAnimMontage;
class USpellData;
class UNiagaraSystem;

//Make abstract
UCLASS(Abstract)
class THEASCENDANCE_API UAbilityData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Ability"))
	FGameplayTag AbilityTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> AbilityIcon = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ECharacterStat UsedStat = ECharacterStat::NONE;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int StatCost = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Cooldown = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool IsTwoHanded = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAnimMontage> AbilityAnimation = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ExcludeBaseStruct))
	TArray<TInstancedStruct<FAbilityModifier>> AbilityModifiers;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ToolTip = "Unneccessary if the abilty does not hit a target, or only an alternative hit niagara (such as an AOE) is wanted"))
	TSoftObjectPtr<UNiagaraSystem> HitNiagara = nullptr;

	EAbilityType AbilityType = EAbilityType::NONE;
};

USTRUCT(BlueprintType)
struct FAbilityTableData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Ability"))
	FGameplayTag AbilityTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AbilityName = "";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString AbilityDescription = "";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> AbilityIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UAbilityData> AbilityData = nullptr;
};