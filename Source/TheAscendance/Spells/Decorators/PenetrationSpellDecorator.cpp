// Fill out your copyright notice in the Description page of Project Settings.


#include "PenetrationSpellDecorator.h"
#include "TheAscendance/Spells/Interfaces/SpellCaster.h"
#include "TheAscendance/Actors/Projectile/Interfaces/Projectile.h"
#include "TheAscendance/Actors/Projectile/Decorators/PenetrationProjectileDecorator.h"

void UPenetrationSpellDecorator::DecorateProjectile(IProjectile* projectile)
{
	m_DecoratedSpell->DecorateProjectile(projectile);

	if (m_ModifierData == nullptr)
	{
		LOG_ERROR("PenetrationSpellDecorator is missing ModifierData");
		return;
	}

	projectile = UPenetrationProjectileDecorator::Builder(projectile, *m_ModifierData.Get()).Build();
	projectile->SetDecoratedSelf(projectile);
}
