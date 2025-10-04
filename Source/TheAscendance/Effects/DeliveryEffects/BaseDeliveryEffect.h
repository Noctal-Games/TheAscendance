// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Effects/BaseEffect.h"
#include "BaseDeliveryEffect.generated.h"

class UCoreEffectData;

UCLASS(Abstract)
class THEASCENDANCE_API UBaseDeliveryEffect : public UBaseEffect
{
	GENERATED_BODY()
	
protected:
	virtual void ApplyEffect(ISusceptible* target);
	virtual void ScanForTargets(const FVector& origin, float radius, TArray<AActor*>& targets);

protected:
	TWeakObjectPtr<UCoreEffectData> m_EffectToApply = nullptr;
};
