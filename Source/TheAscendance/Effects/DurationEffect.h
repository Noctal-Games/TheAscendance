// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreEffect.h"
#include "DurationEffect.generated.h"

class UDurationEffectData;

UCLASS()
class THEASCENDANCE_API UDurationEffect : public UCoreEffect
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
	virtual void ProcessAffectedStat() override;

private:
	TWeakObjectPtr<UDurationEffectData> m_EffectData = nullptr;
	FTimerHandle m_TimerHandle;

	bool m_IsActive = false;
};
