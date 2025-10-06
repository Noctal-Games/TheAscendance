// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpellDecorator.h"
#include "TrailSpellDecorator.generated.h"

struct FTrailSpellModifier;

UCLASS()
class THEASCENDANCE_API UTrailSpellDecorator : public USpellDecorator
{
	GENERATED_BODY()

public:
	class THEASCENDANCE_API Builder
	{
		TWeakObjectPtr<UTrailSpellDecorator> m_Decorator = nullptr;

	public:
		Builder(ISpell* decorator, const FTrailSpellModifier& modifierData)
		{
			m_Decorator = NewObject<UTrailSpellDecorator>();
			m_Decorator->Decorate(decorator);

			m_Decorator->m_ModifierData = MakeShared<FTrailSpellModifier>(modifierData);
		}
		ISpell* Build()
		{
			if (m_Decorator.IsValid() == false)
			{
				LOG_ERROR("Tried to build TrailSpellDecorator without initialising it.")
				return nullptr;
			}

			return m_Decorator.Get();
		}
	};

	virtual void DecorateProjectile(IProjectile* projectile) override;

private:
	TSharedPtr<FTrailSpellModifier> m_ModifierData = nullptr;
};
