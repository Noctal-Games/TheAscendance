// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "TheAscendance/Abilities/Enums/AbilitySlot.h"
#include "Ability.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAbility : public UInterface
{
	GENERATED_BODY()
};

class UAbilityData;
class UAbilityComponent;
struct FAbilityInfo;

class THEASCENDANCE_API IAbility
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Init(UAbilityComponent* ownerComponent, UAbilityData* abilityData) = 0;
	virtual void SetDecoratedSelf(IAbility* decoratedSelf) = 0;

	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void Execute() = 0;
	virtual void TriggerAbility() = 0;

	virtual void OnInputReleased() = 0;

	virtual float PlayAnimMontageOnOwner(UAnimMontage* animation) = 0;

	virtual const FGameplayTag& GetAbilityTag() const = 0;
	virtual bool CanStart() const = 0;

	virtual void Update(float deltaTime) = 0;

	virtual void OnOverlap(AActor* overlapActor, const FVector& spellOverlapLocation, int damage) = 0;
	virtual void OnHit(AActor* hitActor, const FVector& spellHitLocation) = 0;
	virtual void ProcessHit(const FVector& spellHitLocation) = 0;

	virtual void SpawnHitNiagara(const FVector& spellHitLocation) = 0;
	virtual void LoadHitNiagara() = 0;

	virtual TArray<TObjectPtr<AActor>> GetHitActors() = 0;

	virtual void ProcessOverlapDamage(int& damage) = 0;
	virtual void ProcessHitDamage(int& damage, const FVector& targetLocation, const FVector& hitLocation) = 0;

	virtual bool DealDamage(AActor* hitActor, int damage) = 0;
	virtual void ApplyEffects(AActor* hitActor) = 0;

	virtual UAbilityData* GetAbilityData() = 0;
	virtual AActor* GetAbilityOwner() = 0;

	virtual const FAbilityInfo& GetAbilityInfo() const = 0;
	virtual void AddSlot(EAbilitySlot slot) = 0;
};
