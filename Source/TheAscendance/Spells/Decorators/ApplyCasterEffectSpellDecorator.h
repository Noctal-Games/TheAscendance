// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpellDecorator.h"
#include "TheAscendance/Spells/Structs/SpellModifierData.h"
#include "ApplyCasterEffectSpellDecorator.generated.h"

UCLASS()
class THEASCENDANCE_API UApplyCasterEffectSpellDecorator : public USpellDecorator
{
	GENERATED_BODY()
	
public:
	class THEASCENDANCE_API Builder
	{
		TWeakObjectPtr<UApplyCasterEffectSpellDecorator> m_Decorator = nullptr;

	public:
		Builder(ISpell* decorator, const FApplyCasterEffectModifier& modifierData)
		{
			m_Decorator = NewObject<UApplyCasterEffectSpellDecorator>();
			m_Decorator->Decorate(decorator);

			m_Decorator->m_ModifierData = MakeShared<FApplyCasterEffectModifier>(modifierData);
		}
		ISpell* Build()
		{
			if (m_Decorator.IsValid() == false)
			{
				LOG_ERROR("Tried to build ApplyCasterEffectSpellDecorator without initialising it.")
				return nullptr;
			}

			return m_Decorator.Get();
		}
	};

	virtual bool CastSpell() override;

private:
	TSharedPtr<FApplyCasterEffectModifier> m_ModifierData = nullptr;
};
