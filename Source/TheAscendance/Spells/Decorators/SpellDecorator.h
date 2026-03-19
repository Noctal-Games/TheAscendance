// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TheAscendance/Spells/Interfaces/Spell.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "SpellDecorator.generated.h"

class ISpell;
class ISpellCaster;

UCLASS()
class THEASCENDANCE_API USpellDecorator : public UObject, public ISpell
{
	GENERATED_BODY()
	
public:
	void Decorate(ISpell* decorator);

	virtual void SetDecoratedSelf(ISpell* decoratedSelf) override;
	virtual void LoadHitNiagara() override;

	virtual bool CanCast() override;
	virtual bool CastSpell() override;

	virtual void Update(float deltaTime) override;

	virtual void OnOverlap(AActor* overlapActor, const FVector& spellOverlapLocation, int damage) override;
	virtual void OnHit(AActor* hitActor, const FVector& spellHitLocation) override;
	virtual void ProcessHit(const FVector& spellHitLocation) override;
	virtual void SpawnHitNiagara(const FVector& spellHitLocation) override;

	virtual TArray<TObjectPtr<AActor>> GetHitActors() override;

	virtual void Fire(const FVector& direction) override;

	virtual ISpellCaster* GetSpellOwner() override;

	virtual void ProcessOverlapDamage(int& damage) override;
	virtual void ProcessHitDamage(int& damage, const FVector& targetLocation, const FVector& hitLocation) override;

	virtual bool DealDamage(AActor* hitActor, int damage) override;
	virtual void ApplyEffects(AActor* hitActor) override;

	virtual void DecorateProjectile(IProjectile* projectile) override;

protected:
	UPROPERTY()
	TScriptInterface<ISpell> m_DecoratedSpell = nullptr;
};
