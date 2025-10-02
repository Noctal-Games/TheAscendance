// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDeliveryEffect.h"
#include "AOEDeliveryEffect.generated.h"

class UAreaOfEffectDeliveryEffectData;

UCLASS()
class THEASCENDANCE_API UAOEDeliveryEffect : public UBaseDeliveryEffect
{
	GENERATED_BODY()
	
public:
	virtual bool Init(UEffectData* effectData) override;

	virtual void StartEffect(ISusceptible* target, FVector location) override;

	virtual UEffectData* GetEffectData() override;

	virtual void Root() override;
	virtual void UnRoot() override;
	virtual void BeginDestroy() override;

protected: 
	virtual void DoEffect() override;

private:
	TWeakObjectPtr<UAreaOfEffectDeliveryEffectData> m_EffectData = nullptr;

	FVector m_Location = FVector::Zero();

	FTimerHandle m_TimerHandle;

	float m_Timer = 0.0f;
	float m_Interval = 0.1f;
};
