// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "EffectHandlerComponent.generated.h"

class UBaseEffect;
class UCoreEffectData;
class ISusceptible;

USTRUCT()
struct FEffectList
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TObjectPtr<UBaseEffect>> Effects;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEASCENDANCE_API UEffectHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEffectHandlerComponent();

	void Init(ISusceptible* owner);

	void AddEffect(UBaseEffect* effect);
	void ClearEffects();

	// Called every frame
	virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void QueueRemoval(UBaseEffect* effect);
	void RemoveEffects();

private:
	UPROPERTY()
	TScriptInterface<ISusceptible> m_Owner = nullptr;

	UPROPERTY()
	TMap<FGameplayTag, FEffectList> m_Effects;
	UPROPERTY()
	TMap<FGameplayTag, FEffectList> m_EffectsToRemove;
};
