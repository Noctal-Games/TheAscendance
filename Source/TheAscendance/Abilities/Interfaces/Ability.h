// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "Ability.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAbility : public UInterface
{
	GENERATED_BODY()
};

class UAbilityData;
class UAbilityComponent;

class THEASCENDANCE_API IAbility
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Init(UAbilityComponent* ownerComponent, UAbilityData* abilityData) {};
	virtual void SetDecoratedSelf(IAbility* decoratedSelf) {};

	virtual void Start() {};
	virtual void Stop() {};
	virtual void Execute() {};
	virtual void TriggerAbility() {};

	virtual void OnInputReleased() {};

	virtual float PlayAnimMontageOnOwner(UAnimMontage* animation) { return 0.0f; };

	virtual const FGameplayTag& GetAbilityTag() const { return FGameplayTag::EmptyTag; }
	virtual bool CanStart() const { return true; }

	virtual void Update(float deltaTime) {};

	virtual void OnOverlap(AActor* overlapActor, const FVector& spellOverlapLocation, int damage) {};
	virtual void OnHit(AActor* hitActor, const FVector& spellHitLocation) {};
	virtual void ProcessHit(const FVector& spellHitLocation) {};

	virtual void SpawnHitNiagara(const FVector& spellHitLocation) {};

	virtual TArray<TObjectPtr<AActor>> GetHitActors() { return TArray<TObjectPtr<AActor>>(); };

	virtual void ProcessOverlapDamage(int& damage) {};
	virtual void ProcessHitDamage(int& damage, const FVector& targetLocation, const FVector& hitLocation) {};

	virtual bool DealDamage(AActor* hitActor, int damage) { return true; };
	virtual void ApplyEffects(AActor* hitActor) {};

	virtual UAbilityData* GetAbilityData() { return nullptr; };
	virtual AActor* GetAbilityOwner() { return nullptr; };
};
