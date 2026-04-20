// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Projectile.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UProjectile : public UInterface
{
	GENERATED_BODY()
};

class IAbility;
class UProjectileSpellData;

class THEASCENDANCE_API IProjectile
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Init(IAbility* ability, UProjectileSpellData* spellData) {};
	virtual void SetDecoratedSelf(IProjectile* decoratedSelf) {};

	virtual void SetIsActive(bool isActive) {};
	virtual void ApplyForce(const FVector& unitDirection) {};

	virtual void HandleOnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit) {};
	virtual void HandleOnOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) {};
	virtual void HandleOnUpdate(float deltaTime) {};

	virtual void ProcessOverlapDamage(int& damage) {};

	virtual void AddIgnoreActor(AActor* toIgnore) {};

	virtual IAbility* GetAbility() { return nullptr; };

	virtual AActor* GetProjectileActor() { return nullptr; };
	virtual FVector GetProjectileLocation() { return FVector::Zero(); };
};
