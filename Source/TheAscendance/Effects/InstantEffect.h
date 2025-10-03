// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CoreEffect.h"
#include "InstantEffect.generated.h"

class UInstantEffectData;

UCLASS()
class THEASCENDANCE_API UInstantEffect : public UCoreEffect
{
	GENERATED_BODY()
	
public:
	virtual bool Init(UEffectData* effectData) override;

	virtual void StartEffect(ISusceptible* target, FVector location = FVector::Zero()) override;

	virtual UEffectData* GetEffectData() override;

protected:
	virtual void DoEffect() override;
	virtual void ProcessAffectedStat() override;

private:
	TWeakObjectPtr<UInstantEffectData> m_EffectData = nullptr;
};
