// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileFactory.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "Structs/ProjectileModifierData.h"
#include "Enums/ProjectileModifierType.h"
#include "Decorators/TrailProjectileDecorator.h"
#include "Decorators/PenetrationProjectileDecorator.h"
#include "Interfaces/Projectile.h"
#include "BaseProjectile.h"
#include "TheAscendance/Abilities/BaseAbility.h"

ProjectileFactory::ProjectileFactory()
{
	m_SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

IProjectile* ProjectileFactory::CreateProjectile(IAbility* owningAbility, const FVector& spawnLocation, TArray<TInstancedStruct<FProjectileModifier>>* modifiers)
{
	if (owningAbility == nullptr)
	{
		LOG_ERROR("[PROJECTILE FACTORY] Tried to create Projectile with invalid OwningAbility");
		return nullptr;
	}

	UProjectileSpellData* projectileSpellData = Cast<UProjectileSpellData>(owningAbility->GetAbilityData());

	if(projectileSpellData == nullptr)
	{
		LOG_ERROR("[PROJECTILE FACTORY] Tried to create Projectile with OwningAbility that doesn't have valid ProjectileSpellData");
		return nullptr;
	}

	UWorld* world = UCoreFunctionLibrary::GetGameWorld();

	if (world == nullptr)
	{
		return nullptr;
	}

	IProjectile* projectile = world->SpawnActor<ABaseProjectile>(ABaseProjectile::StaticClass(), spawnLocation, FRotator::ZeroRotator, m_SpawnParams);

	if (projectile == nullptr)
	{
		LOG_ERROR("[PROJECTILE FACTORY] Failed to create a BaseProjectile");
		return nullptr;
	}

	if(modifiers != nullptr)
	{
		for (const auto& modifier : *modifiers)
		{
			if (modifier.IsValid() == false)
			{
				continue;
			}

			switch (modifier.Get().ModifierType)
			{
				case EProjectileModifierType::TRAIL:
				{
					if (modifier.GetScriptStruct() != FTrailProjectileModifier::StaticStruct())
					{
						LOG_ERROR("[PROJECTILE FACTORY] A ProjectileModifierType struct with type TRAIL isn't of type TrailProjectileModifier");
						continue;
					}

					const FTrailProjectileModifier& modifierData = modifier.Get<FTrailProjectileModifier>();
					projectile = UTrailProjectileDecorator::Builder(projectile, modifierData).Build();
					break;
				}

				case EProjectileModifierType::PENETRATION:
				{
					if (modifier.GetScriptStruct() != FPenetrationProjectileModifier::StaticStruct())
					{
						LOG_ERROR("[PROJECTILE FACTORY] A ProjectileModifierType struct with type PENETRATION isn't of type PenetrationProjectileModifier");
						continue;
					}

					const FPenetrationProjectileModifier& modifierData = modifier.Get<FPenetrationProjectileModifier>();
					projectile = UPenetrationProjectileDecorator::Builder(projectile, modifierData).Build();
					break;
				}
			}
		}
	}

	projectile->Init(owningAbility, projectileSpellData);
	projectile->SetDecoratedSelf(projectile);
	return projectile;
}
