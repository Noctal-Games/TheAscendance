// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellDecorator.h"

void USpellDecorator::Decorate(ISpell* decorator)
{
	if (decorator == nullptr)
	{
		LOG_ERROR("Tried to decorate Spell with invalid decorator");
		return;
	}

	m_DecoratedSpell = decorator->_getUObject();
}

void USpellDecorator::SetDecoratedSelf(ISpell* decoratedSelf)
{
	if (m_DecoratedSpell == nullptr)
	{
		LOG_ERROR("Spell decorator has invalid DecoratedSpell pointer");
		return;
	}

	return m_DecoratedSpell->SetDecoratedSelf(decoratedSelf);
}

bool USpellDecorator::CanCast()
{
	if (m_DecoratedSpell == nullptr)
	{
		LOG_ERROR("Spell decorator has invalid DecoratedSpell pointer");
		return false;
	}

	return m_DecoratedSpell->CanCast();
}

bool USpellDecorator::CastSpell()
{
	if (m_DecoratedSpell == nullptr)
	{
		LOG_ERROR("Spell decorator has invalid DecoratedSpell pointer");
		return false;
	}

	return m_DecoratedSpell->CastSpell();
}

void USpellDecorator::Update(float deltaTime)
{
	if (m_DecoratedSpell == nullptr)
	{
		LOG_ERROR("Spell decorator has invalid DecoratedSpell pointer");
		return;
	}

	m_DecoratedSpell->Update(deltaTime);
}

void USpellDecorator::OnOverlap(AActor* overlapActor, FVector spellOverlapLocation, int damage)
{
	if (m_DecoratedSpell == nullptr)
	{
		LOG_ERROR("Spell decorator has invalid DecoratedSpell pointer");
		return;
	}

	m_DecoratedSpell->OnOverlap(overlapActor, spellOverlapLocation, damage);
}

void USpellDecorator::OnHit(AActor* hitActor, FVector spellHitLocation)
{
	if (m_DecoratedSpell == nullptr)
	{
		LOG_ERROR("Spell decorator has invalid DecoratedSpell pointer");
		return;
	}

	m_DecoratedSpell->OnHit(hitActor, spellHitLocation);
}

void USpellDecorator::ProcessHit(FVector spellHitLocation)
{
	if (m_DecoratedSpell == nullptr)
	{
		LOG_ERROR("Spell decorator has invalid DecoratedSpell pointer");
		return;
	}

	m_DecoratedSpell->ProcessHit(spellHitLocation);
}

TArray<TObjectPtr<AActor>> USpellDecorator::GetHitActors()
{
	if (m_DecoratedSpell == nullptr)
	{
		LOG_ERROR("Spell decorator has invalid DecoratedSpell pointer");
		return TArray<TObjectPtr<AActor>>();
	}

	return m_DecoratedSpell->GetHitActors();
}


void USpellDecorator::Fire(FVector direction)
{
	if (m_DecoratedSpell == nullptr)
	{
		LOG_ERROR("Spell decorator has invalid DecoratedSpell pointer");
		return;
	}

	m_DecoratedSpell->Fire(direction);
}

ISpellCaster* USpellDecorator::GetSpellOwner()
{
	if (m_DecoratedSpell == nullptr)
	{
		LOG_ERROR("Spell decorator has invalid DecoratedSpell pointer");
		return nullptr;
	}

	return m_DecoratedSpell->GetSpellOwner();
}

void USpellDecorator::ProcessOverlapDamage(int& damage)
{
	if (m_DecoratedSpell == nullptr)
	{
		LOG_ERROR("Spell decorator has invalid DecoratedSpell pointer");
		return;
	}

	m_DecoratedSpell->ProcessOverlapDamage(damage);
}

void USpellDecorator::ProcessHitDamage(int& damage, FVector targetLocation, FVector hitLocation)
{
	if (m_DecoratedSpell == nullptr)
	{
		LOG_ERROR("Spell decorator has invalid DecoratedSpell pointer");
		return;
	}

	m_DecoratedSpell->ProcessHitDamage(damage, targetLocation, hitLocation);
}

void USpellDecorator::DealDamage(AActor* hitActor, int damage)
{
	if (m_DecoratedSpell == nullptr)
	{
		LOG_ERROR("Spell decorator has invalid DecoratedSpell pointer");
		return;
	}

	m_DecoratedSpell->DealDamage(hitActor, damage);
}

void USpellDecorator::DecorateProjectile(IProjectile* projectile)
{
	if (m_DecoratedSpell == nullptr)
	{
		LOG_ERROR("Spell decorator has invalid DecoratedSpell pointer");
		return;
	}

	m_DecoratedSpell->DecorateProjectile(projectile);
}

