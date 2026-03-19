// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpellDecorator.h"
#include "TheAscendance/Spells/Structs/SpellModifierData.h"
#include "SpawnEffectSpellDecorator.generated.h"

UCLASS()
class THEASCENDANCE_API USpawnEffectSpellDecorator : public USpellDecorator
{
	GENERATED_BODY()

public:
	class THEASCENDANCE_API Builder
	{
		TWeakObjectPtr<USpawnEffectSpellDecorator> m_Decorator = nullptr;

	public:
		Builder(ISpell* decorator, const FSpawnEffectModifier& modifierData)
		{
			m_Decorator = NewObject<USpawnEffectSpellDecorator>();
			m_Decorator->Decorate(decorator);

			m_Decorator->m_ModifierData = MakeShared<FSpawnEffectModifier>(modifierData);
		}
		ISpell* Build()
		{
			if (m_Decorator.IsValid() == false)
			{
				LOG_ERROR("Tried to build SpawnEffectSpellDecorator without initialising it.")
				return nullptr;
			}

			return m_Decorator.Get();
		}
	};

	virtual void OnHit(AActor* hitActor, const FVector& spellHitLocation) override;

private:
	TSharedPtr<FSpawnEffectModifier> m_ModifierData = nullptr;
};
