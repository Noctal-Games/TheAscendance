// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityDecorator.h"
#include "TheAscendance/Abilities/Structs/AbilityModifierData.h"
#include "ApplyUserEffectAbilityDecorator.generated.h"

struct FApplyUserEffectAbilityModifier;

UCLASS()
class THEASCENDANCE_API UApplyUserEffectAbilityDecorator : public UAbilityDecorator
{
	GENERATED_BODY()
	
public:
	class THEASCENDANCE_API Builder
	{
		UPROPERTY()
		TWeakObjectPtr<UApplyUserEffectAbilityDecorator> m_Decorator = nullptr;

	public:
		Builder(IAbility* decorator, const FApplyUserEffectAbilityModifier& modifierData)
		{
			m_Decorator = NewObject<UApplyUserEffectAbilityDecorator>();
			m_Decorator->Decorate(decorator);

			m_Decorator->m_ModifierData = MakeShared<FApplyUserEffectAbilityModifier>(modifierData);
		}
		IAbility* Build()
		{
			if (m_Decorator.IsValid() == false)
			{
				LOG_ERROR("[APPLY USER EFFECT ABILITY DECORATOR BUILDER] Tried to build without initialising.")
				return nullptr;
			}

			return m_Decorator.Get();
		}
	};

	virtual void TriggerAbility() override;

private:
	TSharedPtr<FApplyUserEffectAbilityModifier> m_ModifierData = nullptr;
};
