// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityDecorator.h"
#include "TheAscendance/Abilities/Structs/AbilityModifierData.h"
#include "SpawnEffectAbilityDecorator.generated.h"

UCLASS()
class THEASCENDANCE_API USpawnEffectAbilityDecorator : public UAbilityDecorator
{
	GENERATED_BODY()

public:
	class THEASCENDANCE_API Builder
	{
		UPROPERTY()
		TWeakObjectPtr<USpawnEffectAbilityDecorator> m_Decorator = nullptr;

	public:
		Builder(IAbility* decorator, const FSpawnEffectAbilityModifier& modifierData)
		{
			m_Decorator = NewObject<USpawnEffectAbilityDecorator>();
			m_Decorator->Decorate(decorator);

			m_Decorator->m_ModifierData = MakeShared<FSpawnEffectAbilityModifier>(modifierData);
		}
		IAbility* Build()
		{
			if (m_Decorator.IsValid() == false)
			{
				LOG_ERROR("[SPAWN EFFECT ABILITY DECORATOR BUILDER] Tried to build without initialising.")
				return nullptr;
			}

			return m_Decorator.Get();
		}
	};

	virtual void OnHit(AActor* hitActor, const FVector& spellHitLocation) override;

private:
	TSharedPtr<FSpawnEffectAbilityModifier> m_ModifierData = nullptr;
};
