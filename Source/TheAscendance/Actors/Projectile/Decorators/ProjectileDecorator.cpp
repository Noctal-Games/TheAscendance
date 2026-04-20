// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileDecorator.h"

void UProjectileDecorator::Decorate(IProjectile* decorator)
{
	if (decorator == nullptr)
	{
		LOG_ERROR("Tried to decorate Projectile with invalid decorator");
		return;
	}

	m_DecoratedProjectile = decorator->_getUObject();
}

void UProjectileDecorator::Init(IAbility* ability, UProjectileSpellData* spellData)
{
	if (m_DecoratedProjectile == nullptr)
	{
		LOG_ERROR("Projectile decorator has invalid DecoratedProjectile pointer");
		return;
	}

	return m_DecoratedProjectile->Init(ability, spellData);
}

void UProjectileDecorator::SetDecoratedSelf(IProjectile* decoratedSelf)
{
	if (m_DecoratedProjectile == nullptr)
	{
		LOG_ERROR("Projectile decorator has invalid DecoratedProjectile pointer");
		return;
	}

	return m_DecoratedProjectile->SetDecoratedSelf(decoratedSelf);
}

void UProjectileDecorator::SetIsActive(bool isActive)
{
	if (m_DecoratedProjectile == nullptr)
	{
		LOG_ERROR("Projectile decorator has invalid DecoratedProjectile pointer");
		return;
	}

	return m_DecoratedProjectile->SetIsActive(isActive);
}

void UProjectileDecorator::ApplyForce(const FVector& unitDirection)
{
	if (m_DecoratedProjectile == nullptr)
	{
		LOG_ERROR("Projectile decorator has invalid DecoratedProjectile pointer");
		return;
	}

	return m_DecoratedProjectile->ApplyForce(unitDirection);
}

void UProjectileDecorator::HandleOnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit)
{
	if (m_DecoratedProjectile == nullptr)
	{
		LOG_ERROR("Projectile decorator has invalid DecoratedProjectile pointer");
		return;
	}

	return m_DecoratedProjectile->HandleOnHit(hitComp, otherActor, otherComp, normalImpulse, hit);
}

void UProjectileDecorator::HandleOnOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (m_DecoratedProjectile == nullptr)
	{
		LOG_ERROR("Projectile decorator has invalid DecoratedProjectile pointer");
		return;
	}

	return m_DecoratedProjectile->HandleOnOverlap(overlappedComponent, otherActor, otherComp, otherBodyIndex, bFromSweep, sweepResult);
}

void UProjectileDecorator::HandleOnUpdate(float deltaTime)
{
	if (m_DecoratedProjectile == nullptr)
	{
		LOG_ERROR("Projectile decorator has invalid DecoratedProjectile pointer");
		return;
	}

	return m_DecoratedProjectile->HandleOnUpdate(deltaTime);
}

void UProjectileDecorator::ProcessOverlapDamage(int& damage)
{
	if (m_DecoratedProjectile == nullptr)
	{
		LOG_ERROR("Projectile decorator has invalid DecoratedProjectile pointer");
		return;
	}

	return m_DecoratedProjectile->ProcessOverlapDamage(damage);
}

void UProjectileDecorator::AddIgnoreActor(AActor* toIgnore)
{
	if (m_DecoratedProjectile == nullptr)
	{
		LOG_ERROR("Projectile decorator has invalid DecoratedProjectile pointer");
		return;
	}

	return m_DecoratedProjectile->AddIgnoreActor(toIgnore);
}

IAbility* UProjectileDecorator::GetAbility()
{
	if (m_DecoratedProjectile == nullptr)
	{
		LOG_ERROR("Projectile decorator has invalid DecoratedProjectile pointer");
		return nullptr;
	}

	return m_DecoratedProjectile->GetAbility();
}

AActor* UProjectileDecorator::GetProjectileActor()
{
	if (m_DecoratedProjectile == nullptr)
	{
		LOG_ERROR("Projectile decorator has invalid DecoratedProjectile pointer");
		return nullptr;
	}

	return m_DecoratedProjectile->GetProjectileActor();
}

FVector UProjectileDecorator::GetProjectileLocation()
{
	if (m_DecoratedProjectile == nullptr)
	{
		LOG_ERROR("Projectile decorator has invalid DecoratedProjectile pointer");
		return FVector::Zero();
	}

	return m_DecoratedProjectile->GetProjectileLocation();
}
