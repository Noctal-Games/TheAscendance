// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDeliveryEffect.h"
#include "TheAscendance/Characters/Interfaces/Susceptible.h"
#include "ChainDeliveryEffect.generated.h"

class UChainDeliveryEffectData;

USTRUCT()
struct FChainBounce
{
	GENERATED_BODY()

	UPROPERTY()
	TScriptInterface<ISusceptible> Source = nullptr;
	float Radius = 0.0f;
	int BounceIndex = 0;

	FChainBounce() {}
	FChainBounce(ISusceptible* source, float radius, int32 index) : Source(source == nullptr ? nullptr : source->_getUObject()), Radius(radius), BounceIndex(index) {}
};

UCLASS()
class THEASCENDANCE_API UChainDeliveryEffect : public UBaseDeliveryEffect
{
	GENERATED_BODY()
	
public:
	virtual bool Init(UEffectData* effectData) override;

	virtual void StartEffect(ISusceptible* target, FVector location = FVector::Zero()) override;

	virtual UEffectData* GetEffectData() override;

	virtual void Root() override;
	virtual void UnRoot() override;
	virtual void BeginDestroy() override;

private:
	void ProcessNextBounce();

private:
	TWeakObjectPtr<UChainDeliveryEffectData> m_EffectData = nullptr;

	TQueue<FChainBounce> m_BounceQueue;
	TArray<TWeakObjectPtr<UObject>> m_AffectedTargets;

	FTimerHandle m_BounceTimerHandle;
};
