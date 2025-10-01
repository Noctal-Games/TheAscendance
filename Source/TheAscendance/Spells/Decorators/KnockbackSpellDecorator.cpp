// Fill out your copyright notice in the Description page of Project Settings.


#include "KnockbackSpellDecorator.h"
#include "TheAscendance/Spells/Interfaces/SpellCaster.h"

bool UKnockbackSpellDecorator::CastSpell()
{
	if (m_DecoratedSpell == nullptr)
	{
		LOG_ERROR("KnoackbackSpellDecorator has no decorated spell");
		return false;
	}

	FVector unitDirection = m_DecoratedSpell->GetSpellOwner()->GetCastStartForward();
	unitDirection.Normalize();

	Fire(unitDirection);

	return true;
}

void UKnockbackSpellDecorator::Fire(FVector direction)
{
	m_DecoratedSpell->Fire(direction);

	LOG_ONSCREEN(-1, 1.0f, FColor::Cyan, "Knockback Decorator: Fire");
}
