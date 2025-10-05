// Fill out your copyright notice in the Description page of Project Settings.


#include "TrailSpellDecorator.h"
#include "TheAscendance/Spells/Interfaces/SpellCaster.h"
#include "TheAscendance/Actors/Projectile/Interfaces/Projectile.h"
#include "TheAscendance/Actors/Projectile/Decorators/TrailProjectileDecorator.h"

void UTrailSpellDecorator::DecorateProjectile(IProjectile* projectile)
{
	m_DecoratedSpell->DecorateProjectile(projectile);

	if (m_ModifierData == nullptr)
	{
		LOG_ERROR("TrailSpellDecorator is missing ModifierData");
		return;
	}

	projectile = UTrailProjectileDecorator::Builder(projectile, *m_ModifierData.Get()).Build();
	projectile->SetDecoratedSelf(projectile);
}
