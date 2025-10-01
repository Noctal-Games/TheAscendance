// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Projectile.h"
#include "BaseProjectile.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UProjectileMovementComponent;
class UProjectileSpellData;
class ISpell;

UCLASS()
class THEASCENDANCE_API ABaseProjectile : public AActor, public IProjectile
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

	void Init(ISpell* spell, UProjectileSpellData* spellData);
	void AddIgnoreActor(AActor* toIgnore);
	void SetIsActive(bool isActive);
	void ApplyForce(FVector unitDirection);

	virtual void SetDecoratedSelf(IProjectile* decoratedSelf) override;

	virtual void HandleOnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit) override;
	virtual void HandleOnOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) override;
	virtual void HandleOnUpdate(float deltaTime) override;

	virtual void ProcessOverlapDamage(int& damage) override;

	virtual ISpell* GetSpell() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit);
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	// Called every frame
	virtual void Tick(float deltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> m_StaticMeshComponent = nullptr;
	UPROPERTY()
	TObjectPtr<USphereComponent> m_Collider = nullptr;
	UPROPERTY()
	TObjectPtr<UProjectileMovementComponent> m_MovementComponent = nullptr;

	UPROPERTY()
	TScriptInterface<IProjectile> m_DecoratedSelf = nullptr;

	UPROPERTY()
	TScriptInterface<ISpell> m_Spell = nullptr;

	TWeakObjectPtr<UProjectileSpellData> m_SpellData = nullptr;

	TWeakObjectPtr<AActor> m_IgnoredOwner = nullptr;

	FVector m_StartPos = FVector::ZeroVector;
	float m_MaxTravelDistance = 0.0f;

	bool m_IsActive = false;
};
