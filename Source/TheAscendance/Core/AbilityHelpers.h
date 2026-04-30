// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TheAscendance/Actors/Projectile/ProjectileFactory.h"
#include "AbilityHelpers.generated.h"

class IProjectile;
class ProjectileFactory;
class IAbility;

UCLASS()
class THEASCENDANCE_API UAbilityHelpers : public UObject
{
	GENERATED_BODY()
	
public:
	static IProjectile* SpawnProjectile(IAbility* owningAbility, const FVector& spawnLocation = FVector::ZeroVector, TArray<TInstancedStruct<FProjectileModifier>>* modifiers = nullptr);

public:
	static constexpr int MaxAbilities = 4;
	static enum EAbilitySlot
	{
		MAINHAND_PRIMARY = 1,
		MAINHAND_ALT = 2,
		OFFHAND_PRIMARY = 3,
		OFFHAND_ALT = 4
	};

private:
	static TUniquePtr<ProjectileFactory> m_ProjectileFactory;
};
