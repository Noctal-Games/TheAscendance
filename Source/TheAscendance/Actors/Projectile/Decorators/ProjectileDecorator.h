// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Actors/Projectile/Interfaces/Projectile.h"
#include "ProjectileDecorator.generated.h"

class IProjectile;

UCLASS()
class THEASCENDANCE_API UProjectileDecorator : public UObject, public IProjectile
{
	GENERATED_BODY()
	
public:
	void Decorate(IProjectile* decorator);
	virtual void SetDecoratedSelf(IProjectile* decoratedSelf) override;

	virtual void HandleOnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit) override;
	virtual void HandleOnOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) override;
	virtual void HandleOnUpdate(float deltaTime) override;

	virtual void ProcessOverlapDamage(int& damage) override;

	virtual ISpell* GetSpell() override;
protected:
	UPROPERTY()
	TScriptInterface<IProjectile> m_DecoratedProjectile = nullptr;
};
