// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEffect.h"
#include "OverTimeEffect.generated.h"

class UOverTimeEffectData;

UCLASS()
class THEASCENDANCE_API UOverTimeEffect : public UBaseEffect
{
	GENERATED_BODY()
	
public:
	virtual bool Init(UEffectData* effectData) override;

	virtual void StartEffect(ISusceptible* target) override;
	virtual void Update(float deltaTime) override;

	virtual UEffectData* GetEffectData() override;

private:
	TWeakObjectPtr<UOverTimeEffectData> m_EffectData = nullptr;

	float m_Timer = 0.0f;
	float m_IntervalTimer = 0.0f;
};
