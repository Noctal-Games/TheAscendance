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

private:
	static TUniquePtr<ProjectileFactory> m_ProjectileFactory;
};
