// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpellDecorator.h"
#include "TheAscendance/Spells/Structs/SpellModifierData.h"
#include "KnockbackSpellDecorator.generated.h"

struct FKnockbackSpellModifier;

UCLASS()
class THEASCENDANCE_API UKnockbackSpellDecorator : public USpellDecorator
{
	GENERATED_BODY()
	
public:
	class THEASCENDANCE_API Builder
	{
		TWeakObjectPtr<UKnockbackSpellDecorator> m_Decorator = nullptr;

	public:
		Builder(ISpell* decorator, const FKnockbackSpellModifier& modifierData)
		{
			m_Decorator = NewObject<UKnockbackSpellDecorator>();
			m_Decorator->Decorate(decorator);
			m_Decorator->m_ModifierData = MakeShared<FKnockbackSpellModifier>(modifierData);
		}
		ISpell* Build()
		{
			if (m_Decorator.IsValid() == false)
			{
				LOG_ERROR("Tried to build KnockbackSpellDecorator without initialising it.")
				return nullptr;
			}

			return m_Decorator.Get();
		}
	};

	virtual void OnHit(AActor* hitActor, FVector spellHitLocation) override;

private:
	TSharedPtr<FKnockbackSpellModifier> m_ModifierData = nullptr;
};
