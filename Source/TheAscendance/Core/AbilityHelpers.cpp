// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityHelpers.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Actors/Projectile/ProjectileFactory.h"

TUniquePtr<ProjectileFactory> UAbilityHelpers::m_ProjectileFactory = nullptr;

IProjectile* UAbilityHelpers::SpawnProjectile(IAbility* owningAbility, const FVector& spawnLocation, TArray<TInstancedStruct<FProjectileModifier>>* modifiers)
{
	if (m_ProjectileFactory == nullptr)
	{
		m_ProjectileFactory = MakeUnique<ProjectileFactory>();
		LOG_INFO("[ABILITY HELPERS] Created Projectile Factory");

		if (m_ProjectileFactory == nullptr)
		{
			LOG_ERROR("[ABILITY HELPERS] Failed to create Projectile Factory");
			return nullptr;
		}
	}

	return m_ProjectileFactory->CreateProjectile(owningAbility, spawnLocation, modifiers);
}
