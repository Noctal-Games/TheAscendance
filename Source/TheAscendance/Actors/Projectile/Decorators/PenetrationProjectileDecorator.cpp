// Fill out your copyright notice in the Description page of Project Settings.


#include "PenetrationProjectileDecorator.h"
#include "TheAscendance/Spells/Interfaces/Spell.h"

void UPenetrationProjectileDecorator::HandleOnOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (m_PenetrateCount >= m_PenetrateLimit)
	{
		m_DecoratedProjectile->HandleOnOverlap(overlappedComponent, otherActor, otherComp, otherBodyIndex, bFromSweep, sweepResult);
		return;
	}

	int damage = 0;
	ProcessOverlapDamage(damage);

	if (ISpell* spell = GetSpell())
	{
		spell->OnOverlap(otherActor, otherActor->GetActorLocation(), damage);

	}

	m_PenetrateCount += 1;
	LOG_ONSCREEN(-1, 3.0f, FColor::Blue, "%i", m_PenetrateCount);
}

void UPenetrationProjectileDecorator::ProcessOverlapDamage(int& damage)
{
	damage -= m_DamageLossPerHit * m_PenetrateCount;

	m_DecoratedProjectile->ProcessOverlapDamage(damage);
}
