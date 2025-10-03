// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Spell.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USpell : public UInterface
{
	GENERATED_BODY()
};

class USpellData;
class ISpellCaster;
class IProjectile;

class THEASCENDANCE_API ISpell
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void Init(USpellData* spellData, ISpellCaster* spellOwner) {};
	virtual void SetDecoratedSelf(ISpell* decoratedSelf) {};

	virtual bool CanCast() { return false; };
	virtual bool CastSpell() { return false; };

	virtual void Update(float deltaTime) {};

	virtual void OnOverlap(AActor* overlapActor, FVector spellOverlapLocation, int damage) {};
	virtual void OnHit(AActor* hitActor, FVector spellHitLocation) {};
	virtual void ProcessHit(FVector spellHitLocation) {};
	virtual TArray<TObjectPtr<AActor>> GetHitActors() { return TArray<TObjectPtr<AActor>>(); };

	virtual void Fire(FVector direction) {};

	virtual ISpellCaster* GetSpellOwner() { return nullptr; };

	virtual void ProcessOverlapDamage(int& damage) {};
	virtual void ProcessHitDamage(int& damage, FVector targetLocation, FVector hitLocation) {};

	virtual void DealDamage(AActor* hitActor, int damage) {};
	virtual void DecorateProjectile(IProjectile* projectile) {};

};
