// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Effects/Enums/EffectType.h"
#include "TheAscendance/Characters/Enums/CharacterStat.h"
#include "InstancedStruct.h"
#include "GameplayTagContainer.h"
#include "EffectData.generated.h"

class UNiagaraSystem;

UCLASS(Abstract, BlueprintType)
class THEASCENDANCE_API UEffectData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag EffectTag;

	EEffectType EffectType = EEffectType::INSTANT;
};

UCLASS(Abstract, BlueprintType)
class THEASCENDANCE_API UCoreEffectData : public UEffectData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ECharacterStat AffectedStat = ECharacterStat::NONE;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Potency = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UNiagaraSystem> EffectNiagara = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool CanStack = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "CanStack == true", EditConditionHides))
	int StackCap = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> EffectIcon = nullptr;
};

UCLASS(BlueprintType)
class THEASCENDANCE_API UInstantEffectData : public UCoreEffectData
{
	GENERATED_BODY()

public:
	UInstantEffectData()
	{
		EffectType = EEffectType::INSTANT;
	}
};

UCLASS(BlueprintType)
class THEASCENDANCE_API UOverTimeEffectData : public UCoreEffectData
{
	GENERATED_BODY()

public:
	UOverTimeEffectData()
	{
		EffectType = EEffectType::OVERTIME;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Duration = 0.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float EffectInterval = 0.0f;
};

UCLASS(BlueprintType)
class THEASCENDANCE_API UDurationEffectData : public UCoreEffectData
{
	GENERATED_BODY()

public:
	UDurationEffectData()
	{
		EffectType = EEffectType::DURATION;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Duration = 0.0f;
};

UCLASS(Abstract, BlueprintType)
class THEASCENDANCE_API UDeliveryEffectData : public UEffectData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DeliveryRange = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UNiagaraSystem> DeliveryNiagara = nullptr;
};

UCLASS(BlueprintType)
class THEASCENDANCE_API UChainDeliveryEffectData : public UDeliveryEffectData
{
	GENERATED_BODY()

public:
	UChainDeliveryEffectData()
	{
		EffectType = EEffectType::CHAIN;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = 0.1f))
	float BounceDelay = 0.1f;
};

USTRUCT(BlueprintType)
struct FEffectTableData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag EffectTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString EffectDescription = "";
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UEffectData> EffectData = nullptr;
};