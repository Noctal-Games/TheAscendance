// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreEffect.h"
#include "OverTimeEffect.generated.h"

class UOverTimeEffectData;

UCLASS()
class THEASCENDANCE_API UOverTimeEffect : public UCoreEffect
{
	GENERATED_BODY()
	
public:
	virtual bool Init(UEffectData* effectData) override;

	virtual void StartEffect(ISusceptible* target, FVector location = FVector::Zero()) override;
	virtual void EndEffect() override;

	virtual void ResetEffect() override;

	virtual UEffectData* GetEffectData() override;

protected:
	virtual void DoEffect() override;

private:
	TWeakObjectPtr<UOverTimeEffectData> m_EffectData = nullptr;

	float m_Timer = 0.0f;
	float m_Interval = 0.0f;

	FTimerHandle m_TimerHandle;
};
