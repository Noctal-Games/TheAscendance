// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileDecorator.h"
#include "TheAscendance/Spells/Structs/SpellModifierData.h"
#include "GameplayTags.h"
#include "TrailProjectileDecorator.generated.h"

class UAreaOfEffectDeliveryEffectData;

UCLASS()
class THEASCENDANCE_API UTrailProjectileDecorator : public UProjectileDecorator
{
	GENERATED_BODY()
	
public:
	class THEASCENDANCE_API Builder
	{
		TWeakObjectPtr<UTrailProjectileDecorator> m_Decorator = nullptr;

	public:
		Builder(IProjectile* decorator, const FTrailSpellModifier& modifierData)
		{
			m_Decorator = NewObject<UTrailProjectileDecorator>();
			m_Decorator->Decorate(decorator);

			m_Decorator->m_EffectTag = modifierData.TrailEffectTag;
		}
		IProjectile* Build()
		{
			if (m_Decorator.IsValid() == false)
			{
				LOG_ERROR("Tried to build TrailProjectileDecorator without initialising it.")
				return nullptr;
			}

			m_Decorator->Init();

			return m_Decorator.Get();
		}
	};

	virtual void HandleOnUpdate(float deltaTime) override;

private: 
	void Init();

private:
	TWeakObjectPtr<UAreaOfEffectDeliveryEffectData> m_TrailEffectData = nullptr;
	FGameplayTag m_EffectTag;

	FVector m_LastLocation = FVector::ZeroVector;

	float m_SpawnInterval = 0.0f;
};
