// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Effects/BaseEffect.h"
#include "TheAscendance/Characters/Interfaces/Susceptible.h"
#include "ChainDeliveryEffect.generated.h"

class UChainDeliveryEffectData;
class UCoreEffectData;

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
class THEASCENDANCE_API UChainDeliveryEffect : public UBaseEffect
{
	GENERATED_BODY()
	
public:
	virtual bool Init(UEffectData* effectData) override;

	virtual void StartEffect(ISusceptible* target) override;

	virtual UEffectData* GetEffectData() override;

	void Root();
	void UnRoot();
	virtual void BeginDestroy() override;

private:
	void ProcessNextBounce();
	void ApplyEffect(ISusceptible* target);
	void FindCharactersInRadius(const FVector& origin, float radius, TArray<ISusceptible*>& targets);

private:
	TWeakObjectPtr<UChainDeliveryEffectData> m_EffectData = nullptr;
	TWeakObjectPtr<UCoreEffectData> m_EffectToApply = nullptr;

	TQueue<FChainBounce> m_BounceQueue;
	TArray<TWeakObjectPtr<UObject>> m_AffectedTargets;

	FTimerHandle m_BounceTimerHandle;

	bool m_HasEnded = false;
};
