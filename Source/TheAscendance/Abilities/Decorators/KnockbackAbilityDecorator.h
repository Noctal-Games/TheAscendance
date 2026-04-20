// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "AbilityDecorator.h"
#include "TheAscendance/Abilities/Structs/AbilityModifierData.h"
#include "KnockbackAbilityDecorator.generated.h"

struct FKnockbackAbilityModifier;

UCLASS()
class THEASCENDANCE_API UKnockbackAbilityDecorator : public UAbilityDecorator
{
	GENERATED_BODY()
	
public:
	class THEASCENDANCE_API Builder
	{
		UPROPERTY()
		TWeakObjectPtr<UKnockbackAbilityDecorator> m_Decorator = nullptr;

	public:
		Builder(IAbility* decorator, const FKnockbackAbilityModifier& modifierData)
		{
			m_Decorator = NewObject<UKnockbackAbilityDecorator>();
			m_Decorator->Decorate(decorator);
			m_Decorator->m_ModifierData = MakeShared<FKnockbackAbilityModifier>(modifierData);
		}
		IAbility* Build()
		{
			if (m_Decorator.IsValid() == false)
			{
				LOG_ERROR("[KNOCKBACK ABILITY DECORATOR BUILDER] Tried to build without initialising.")
				return nullptr;
			}

			return m_Decorator.Get();
		}
	};

	virtual void OnHit(AActor* hitActor, const FVector& spellHitLocation) override;

private:
	TSharedPtr<FKnockbackAbilityModifier> m_ModifierData = nullptr;
};
