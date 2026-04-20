// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TheAscendance/Abilities/BaseAbility.h"
#include "BaseSpell.generated.h"

class ISpellCaster;
class USpellData;
class UNiagaraSystem;

UCLASS(Abstract)
class THEASCENDANCE_API UBaseSpell : public UBaseAbility
{
	GENERATED_BODY()
	
public:
	virtual void OnOverlap(AActor* overlapActor, const FVector& spellOverlapLocation, int damage) override;
	virtual void OnHit(AActor* hitActor, const FVector& spellHitLocation) override;
	virtual void ProcessHit(const FVector& spellHitLocation) override;

	virtual void SpawnHitNiagara(const FVector& spellHitLocation) override;

	virtual bool DealDamage(AActor* hitActor, int damage) override;

	virtual TArray<TObjectPtr<AActor>> GetHitActors() override;

	virtual void ApplyEffects(AActor* hitActor) override;

protected:
	UPROPERTY()
	TScriptInterface<ISpellCaster> m_SpellOwner = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<AActor>> m_HitActors;

	UPROPERTY()
	TSoftObjectPtr<UNiagaraSystem> m_HitNiagara = nullptr;

private:
	float m_Cooldown = 0.0f;
	float m_CooldownTimer = 0.0f;
};
