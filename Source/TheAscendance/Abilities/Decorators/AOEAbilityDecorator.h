// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityDecorator.h"
#include "TheAscendance/Abilities/Structs/AbilityModifierData.h"
#include "AOEAbilityDecorator.generated.h"

class UNiagaraSystem;

UCLASS()
class THEASCENDANCE_API UAOEAbilityDecorator : public UAbilityDecorator
{
	GENERATED_BODY()
	
public:
	class THEASCENDANCE_API Builder
	{
		UPROPERTY()
		TWeakObjectPtr<UAOEAbilityDecorator> m_Decorator = nullptr;

	public:
		Builder(IAbility* decorator, const FAreaOfEffectModifier& modifierData)
		{
			m_Decorator = NewObject<UAOEAbilityDecorator>();
			m_Decorator->Decorate(decorator);

			m_Decorator->m_ModifierData = MakeShared<FAreaOfEffectModifier>(modifierData);
		}
		IAbility* Build()
		{
			if (m_Decorator.IsValid() == false)
			{
				LOG_ERROR("[AOE ABILITY DECORATOR BUILDER] Tried to build without initialising.")
				return nullptr;
			}

			return m_Decorator.Get();
		}
	};

	virtual void OnHit(AActor* hitActor, const FVector& spellHitLocation) override;
	virtual void ProcessHit(const FVector& spellHitLocation) override;
	virtual void ProcessHitDamage(int& damage, const FVector& targetLocation, const FVector& hitLocation) override;
	virtual void SpawnHitNiagara(const FVector& spellHitLocation) override;

private:
	TSharedPtr<FAreaOfEffectModifier> m_ModifierData = nullptr;

	UPROPERTY()
	TSoftObjectPtr<UNiagaraSystem> m_AOEHitNiagara = nullptr;
};
