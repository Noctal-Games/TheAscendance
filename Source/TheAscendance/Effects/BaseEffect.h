// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BaseEffect.generated.h"

class ISusceptible;
class UEffectData;

DECLARE_DELEGATE_OneParam(FOnEffectEnd, UBaseEffect*);

UCLASS()
class THEASCENDANCE_API UBaseEffect : public UObject
{
	GENERATED_BODY()
	
public:
	virtual bool Init(UEffectData* effectData);

	virtual void StartEffect(ISusceptible* target);
	virtual void Update(float deltaTime);
	virtual void EndEffect();

	virtual UEffectData* GetEffectData();

public:
	FOnEffectEnd OnEffectEnd;

protected:
	UPROPERTY()
	TScriptInterface<ISusceptible> m_Target = nullptr;

	bool m_HasEnded = false;
};
