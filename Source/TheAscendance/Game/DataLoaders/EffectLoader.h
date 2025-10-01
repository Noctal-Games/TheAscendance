// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TheAscendance/Effects/EffectFactory.h"
#include "TheAscendance/Core/GameplayTagHelpers.h"
#include "EffectLoader.generated.h"

class UBaseEffect;
class UEffectData;

UCLASS()
class THEASCENDANCE_API UEffectLoader : public UObject
{
	GENERATED_BODY()

public:
	void Init();

	UBaseEffect* CreateEffectFromTag(const FGameplayTag& effectTag);
	UBaseEffect* CreateEffectFromEffectData(UEffectData* effectData);

	UEffectData* LoadEffectData(const FGameplayTag& effectTag);

private:
	UPROPERTY()
	TObjectPtr<UDataTable> m_EffectTable = nullptr;

	TUniquePtr<EffectFactory> m_EffectFactory = nullptr;
};
