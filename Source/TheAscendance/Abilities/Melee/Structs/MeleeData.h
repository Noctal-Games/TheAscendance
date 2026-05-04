// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Abilities/Structs/AbilityData.h"
#include "MeleeData.generated.h"

USTRUCT()
struct FMeleeAbilityData
{
	GENERATED_BODY()

	FMeleeAbilityData() {};

	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Ability.Melee"))
	FGameplayTag AbilityTag;
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Spell abilities override if assigned to the same slot"))
	bool DoesSpellOverride = false;
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Bonus flat damage, will be applied to all damage sources in the ability"))
	float BonusDamage = 0.0f;
};

UCLASS(BlueprintType)
class THEASCENDANCE_API UMeleeData : public UAbilityData
{
	GENERATED_BODY()

public:
	UMeleeData()
	{
		AbilityType = EAbilityType::MELEE;
	}
};

USTRUCT(BlueprintType)
struct FMeleeTableData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Ability.Melee"))
	FGameplayTag MeleeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName MeleeName = "";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString MeleeDescription = "";

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UMeleeData> MeleeData = nullptr;
};