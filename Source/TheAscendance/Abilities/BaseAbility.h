// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/Ability.h"
#include "BaseAbility.generated.h"

class UAbilityData;
class UAbilityComponent;
class UAnimMontage;
class UNiagaraSystem;

UCLASS()
class THEASCENDANCE_API UBaseAbility : public UObject, public IAbility
{
	GENERATED_BODY()
	
public:
	virtual void Init(UAbilityComponent* ownerComponent, UAbilityData* abilityData) override;
	virtual void SetDecoratedSelf(IAbility* decoratedSelf) override;

	//Checks whether or not the ability has to be charged or can execute immediately.
	virtual void Start() override;
	//Stops the ability. Clears up all timer handles, etc.
	virtual void Stop() override;
	//Starts the ability animation
	virtual void Execute() override;

	//Triggered by animations, this is the actual ability start event. So when a spell is cast, a melee collision is activated, etc.
	virtual void TriggerAbility() override;

	virtual void OnInputReleased() override;

	virtual void OnOverlap(AActor* overlapActor, const FVector& overlapLocation, int damage) override;
	virtual void OnHit(AActor* hitActor, const FVector& hitLocation) override;
	virtual void ProcessHit(const FVector& hitLocation) override;

	virtual bool DealDamage(AActor* hitActor, int damage) override;

	virtual void SpawnHitNiagara(const FVector& hitLocation) override;
	virtual void LoadHitNiagara() override;

	virtual const FGameplayTag& GetAbilityTag() const override;

	virtual TArray<TObjectPtr<AActor>> GetHitActors() override;

	virtual float PlayAnimMontageOnOwner(UAnimMontage* animation) override;
	virtual UAbilityData* GetAbilityData() override;
	virtual AActor* GetAbilityOwner() override;

	virtual bool CanStart() const override;
	virtual void Update(float deltaTime) override;

protected:
	virtual void AffectOwnerStat();

protected:
	UPROPERTY()
	TObjectPtr<UAbilityData> m_AbilityData = nullptr;
	UPROPERTY()
	TScriptInterface<IAbility> m_DecoratedSelf = nullptr;
	UPROPERTY()
	TWeakObjectPtr<UAbilityComponent> m_OwnerComponent = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> m_HitActors;
private:
	UPROPERTY()
	TSoftObjectPtr<UAnimMontage> m_AbilityAnimation = nullptr;

	FTimerHandle m_AbilityDurationHandle;

	UPROPERTY()
	TSoftObjectPtr<UNiagaraSystem> m_HitNiagara = nullptr;

	float m_Cooldown = 0.0f;
	float m_CooldownTimer = 0.0f;
};
