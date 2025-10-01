// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpellDecorator.h"
#include "TheAscendance/Spells/Structs/SpellModifierData.h"
#include "AOESpellDecorator.generated.h"

UCLASS()
class THEASCENDANCE_API UAOESpellDecorator : public USpellDecorator
{
	GENERATED_BODY()
	
public:
	class THEASCENDANCE_API Builder
	{
		TWeakObjectPtr<UAOESpellDecorator> m_Decorator = nullptr;

	public:
		Builder(ISpell* decorator, const FAreaOfEffectModifier& modifierData)
		{
			m_Decorator = NewObject<UAOESpellDecorator>();
			m_Decorator->Decorate(decorator);

			m_Decorator->m_ModifierData = MakeShared<FAreaOfEffectModifier>(modifierData);
		}
		ISpell* Build()
		{
			if (m_Decorator.IsValid() == false)
			{
				LOG_ERROR("Tried to build AOESpellDecorator without initialising it.")
				return nullptr;
			}

			return m_Decorator.Get();
		}
	};

	virtual void OnHit(AActor* hitActor, FVector spellHitLocation) override;
	virtual void ProcessHit(FVector spellHitLocation) override;
	virtual void ProcessHitDamage(int& damage, FVector targetLocation, FVector hitLocation) override;

	//Test
	virtual bool CastSpell() override;
	virtual void Fire(FVector direction) override;

private:
	TSharedPtr<FAreaOfEffectModifier> m_ModifierData = nullptr;
};
