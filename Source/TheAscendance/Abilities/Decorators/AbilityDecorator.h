// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Abilities/Interfaces/Ability.h"
#include "AbilityDecorator.generated.h"

class IAbility;

UCLASS(Abstract)
class THEASCENDANCE_API UAbilityDecorator : public UObject, public IAbility
{
	GENERATED_BODY()
	
public:
	void Decorate(IAbility* decorator);

	virtual void Init(UAbilityComponent* ownerComponent, UAbilityData* abilityData) override;
	virtual void SetDecoratedSelf(IAbility* decoratedSelf) override;

	virtual void Start() override;
	virtual void Stop() override;
	virtual void Execute() override;
	virtual void TriggerAbility() override;

	virtual const FGameplayTag& GetAbilityTag() const override;
	virtual bool CanStart() const override;

	virtual void OnInputReleased() override;

	virtual float PlayAnimMontageOnOwner(UAnimMontage* animation) override;

	virtual void Update(float deltaTime) override;

	virtual void OnOverlap(AActor* overlapActor, const FVector& spellOverlapLocation, int damage) override;
	virtual void OnHit(AActor* hitActor, const FVector& spellHitLocation) override;
	virtual void ProcessHit(const FVector& spellHitLocation) override;

	virtual void SpawnHitNiagara(const FVector& spellHitLocation) override;
	virtual void LoadHitNiagara() override;

	virtual TArray<TObjectPtr<AActor>> GetHitActors() override;

	virtual void ProcessOverlapDamage(int& damage) override;
	virtual void ProcessHitDamage(int& damage, const FVector& targetLocation, const FVector& hitLocation) override;

	virtual bool DealDamage(AActor* hitActor, int damage) override;
	virtual void ApplyEffects(AActor* hitActor) override;

	virtual UAbilityData* GetAbilityData() override;
	virtual AActor* GetAbilityOwner() override;

protected:
	UPROPERTY()
	TScriptInterface<IAbility> m_DecoratedAbility = nullptr;
};
