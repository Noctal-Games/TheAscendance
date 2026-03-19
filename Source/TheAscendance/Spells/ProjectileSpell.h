// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"
#include "ProjectileSpell.generated.h"

class UProjectileSpellData;
class IProjectile;

UCLASS()
class THEASCENDANCE_API UProjectileSpell : public UBaseSpell
{
	GENERATED_BODY()
	
public:
	virtual void Init(USpellData* spellData, ISpellCaster* spellOwner) final;
	virtual void LoadHitNiagara() override;
	
	virtual bool CastSpell() override;
	virtual void Fire(const FVector& direction) override;

	virtual void ProcessOverlapDamage(int& damage) override;
	virtual void ProcessHitDamage(int& damage, const FVector& targetLocation, const FVector& hitLocation) override;

	virtual void DecorateProjectile(IProjectile* projectile) override;

	virtual USpellData* GetSpellData() override;

private:
	UPROPERTY()
	TWeakObjectPtr<UProjectileSpellData> m_SpellData = nullptr;
};
