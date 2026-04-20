// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "AbilityDecorator.h"
#include "TheAscendance/Abilities/Structs/AbilityModifierData.h"
#include "ApplyEffectAbilityDecorator.generated.h"

struct FApplyEffectModifier;

UCLASS()
class THEASCENDANCE_API UApplyEffectAbilityDecorator : public UAbilityDecorator
{
	GENERATED_BODY()

public:
	class THEASCENDANCE_API Builder
	{
		UPROPERTY()
		TWeakObjectPtr<UApplyEffectAbilityDecorator> m_Decorator = nullptr;

	public:
		Builder(IAbility* decorator, const FApplyEffectModifier& modifierData)
		{
			m_Decorator = NewObject<UApplyEffectAbilityDecorator>();
			m_Decorator->Decorate(decorator);

			m_Decorator->m_ModifierData = MakeShared<FApplyEffectModifier>(modifierData);
		}
		IAbility* Build()
		{
			if (m_Decorator.IsValid() == false)
			{
				LOG_ERROR("[APPLY EFFECT ABILITY DECORATOR BUILDER] Tried to build without initialising.")
				return nullptr;
			}

			return m_Decorator.Get();
		}
	};

	virtual void ApplyEffects(AActor* hitActor) override;

private:
	TSharedPtr<FApplyEffectModifier> m_ModifierData = nullptr;
};
