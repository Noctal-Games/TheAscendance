// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileDecorator.h"
#include "TheAscendance/Spells/Structs/SpellModifierData.h"
#include "PenetrationProjectileDecorator.generated.h"

UCLASS()
class THEASCENDANCE_API UPenetrationProjectileDecorator : public UProjectileDecorator
{
	GENERATED_BODY()
	
public:
	class THEASCENDANCE_API Builder
	{
		TWeakObjectPtr<UPenetrationProjectileDecorator> m_Decorator = nullptr;

	public:
		Builder(IProjectile* decorator, const FPenetrationSpellModifier& modifierData)
		{
			m_Decorator = NewObject<UPenetrationProjectileDecorator>();
			m_Decorator->Decorate(decorator);

			m_Decorator->m_DamageLossPerHit = modifierData.DamageLossPerHit;
			m_Decorator->m_PenetrateLimit = modifierData.PenetrateLimit;
			m_Decorator->m_PenetrateCount = 0;
		}
		IProjectile* Build()
		{
			if (m_Decorator.IsValid() == false)
			{
				LOG_ERROR("Tried to build PenetrationProjectileDecorator without initialising it.")
				return nullptr;
			}

			return m_Decorator.Get();
		}
	};

	virtual void HandleOnOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult) override;
	virtual void ProcessOverlapDamage(int& damage) override;

private:
	int m_DamageLossPerHit = 0;
	int m_PenetrateLimit = 0;
	int m_PenetrateCount = 0;
};
