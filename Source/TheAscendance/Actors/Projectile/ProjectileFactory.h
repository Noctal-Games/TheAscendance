// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct.h"

class IProjectile;
class UBaseAbility;
struct FProjectileModifier;

class THEASCENDANCE_API ProjectileFactory
{
public:
	ProjectileFactory();
	~ProjectileFactory() = default;

	IProjectile* CreateProjectile(IAbility* owningAbility, const FVector& spawnLocation = FVector::ZeroVector, TArray<TInstancedStruct<FProjectileModifier>>* modifiers = nullptr);

private:
	FActorSpawnParameters m_SpawnParams;
};
