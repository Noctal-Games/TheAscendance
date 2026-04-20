// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"
#include "ProjectileSpell.generated.h"

class UProjectileSpellData;
class IProjectile;
class UNiagaraSystem;

UCLASS()
class THEASCENDANCE_API UProjectileSpell : public UBaseSpell
{
	GENERATED_BODY()
	
public:
	virtual void Init(UAbilityComponent* ownerComponent, UAbilityData* abilityData) override;
	virtual void TriggerAbility() override;

	virtual void ProcessOverlapDamage(int& damage) override;
	virtual void ProcessHitDamage(int& damage, const FVector& targetLocation, const FVector& hitLocation) override;

private:
	UPROPERTY()
	TWeakObjectPtr<UProjectileSpellData> m_SpellData = nullptr;

	UPROPERTY()
	TSoftObjectPtr<UNiagaraSystem> m_ProjectileNiagara = nullptr;
};
