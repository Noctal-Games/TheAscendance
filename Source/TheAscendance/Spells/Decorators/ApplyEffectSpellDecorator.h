// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpellDecorator.h"
#include "ApplyEffectSpellDecorator.generated.h"

struct FApplyEffectModifier;

UCLASS()
class THEASCENDANCE_API UApplyEffectSpellDecorator : public USpellDecorator
{
	GENERATED_BODY()

public:
	class THEASCENDANCE_API Builder
	{
		TWeakObjectPtr<UApplyEffectSpellDecorator> m_Decorator = nullptr;

	public:
		Builder(ISpell* decorator, const FApplyEffectModifier& modifierData)
		{
			m_Decorator = NewObject<UApplyEffectSpellDecorator>();
			m_Decorator->Decorate(decorator);

			m_Decorator->m_ModifierData = MakeShared<FApplyEffectModifier>(modifierData);
		}
		ISpell* Build()
		{
			if (m_Decorator.IsValid() == false)
			{
				LOG_ERROR("Tried to build ApplyEffectSpellDecorator without initialising it.")
				return nullptr;
			}

			return m_Decorator.Get();
		}
	};

	virtual void ApplyEffects(AActor* hitActor) override;

private:
	TSharedPtr<FApplyEffectModifier> m_ModifierData = nullptr;
};
