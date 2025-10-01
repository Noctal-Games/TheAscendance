// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpellDecorator.h"
#include "PenetrationSpellDecorator.generated.h"

struct FPenetrationSpellModifier;

UCLASS()
class THEASCENDANCE_API UPenetrationSpellDecorator : public USpellDecorator
{
	GENERATED_BODY()

public:
	class THEASCENDANCE_API Builder
	{
		TWeakObjectPtr<UPenetrationSpellDecorator> m_Decorator = nullptr;

	public:
		Builder(ISpell* decorator, const FPenetrationSpellModifier& modifierData)
		{
			m_Decorator = NewObject<UPenetrationSpellDecorator>();
			m_Decorator->Decorate(decorator);

			m_Decorator->m_ModifierData = MakeShared<FPenetrationSpellModifier>(modifierData);
		}
		ISpell* Build()
		{
			if (m_Decorator.IsValid() == false)
			{
				LOG_ERROR("Tried to build PenetrationSpellDecorator without initialising it.")
				return nullptr;
			}

			return m_Decorator.Get();
		}
	};

	virtual void DecorateProjectile(IProjectile* projectile) override;

	//Test
	virtual bool CastSpell() override;
	virtual void Fire(FVector direction) override;

private:
	TSharedPtr<FPenetrationSpellModifier> m_ModifierData = nullptr;
};
