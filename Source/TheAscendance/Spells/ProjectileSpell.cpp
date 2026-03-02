// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileSpell.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "Interfaces/SpellCaster.h"
#include "Structs/SpellData.h"
#include "TheAscendance/Actors/Projectile/BaseProjectile.h"
#include "TheAscendance/Characters/Interfaces/Susceptible.h"
#include "TheAscendance/Actors/Projectile/Interfaces/Projectile.h"

void UProjectileSpell::Init(USpellData* spellData, ISpellCaster* spellOwner)
{
	UBaseSpell::Init(spellData, spellOwner);

	if (spellData == nullptr || spellOwner == nullptr)
	{
		return;
	}

	if (UProjectileSpellData* data = Cast<UProjectileSpellData>(spellData))
	{
		m_SpellData = data;
	}
	else
	{
		LOG_ERROR("[PROJECTILE SPELL] Tried to Init ProjectileSpellBase with invalid SpellData");
	}
}

void UProjectileSpell::LoadHitNiagara()
{
	if(m_SpellData.IsValid() == false)
	{
		LOG_ERROR("[PROJECTILE SPELL] Tried to Load HitNiagara for ProjectileSpell with invalid SpellData");
		return;
	}

	m_HitNiagara = m_SpellData->SpellHitNiagara;

	if (m_HitNiagara.IsNull() == true)
	{
		LOG_WARNING("[PROJECTILE SPELL] Tried to Load HitNiagara for ProjectileSpell with invalid SpellHitNiagara");
		return;
	}

	UCoreFunctionLibrary::RequestAsyncLoad(m_HitNiagara.ToSoftObjectPath());
}

bool UProjectileSpell::CastSpell()
{
	if (UBaseSpell::CastSpell() == false)
	{
		return false;
	}

	FVector unitDirection = GetSpellOwner()->GetCastStartForward();
	unitDirection.Normalize();

	Fire(unitDirection);

	return true;
}

void UProjectileSpell::Fire(FVector direction)
{
	if (m_DecoratedSelf == nullptr)
	{
		LOG_ERROR("[PROJECTILE SPELL] Tried to Fire Projectile with invalid DecoratedSelf");
		return;
	}

	if (m_HitNiagara.IsValid() == false)
	{
		m_DecoratedSelf->LoadHitNiagara();
	}

	AActor* owner = m_SpellOwner->GetActor();

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABaseProjectile* projectile = owner->GetWorld()->SpawnActor<ABaseProjectile>(ABaseProjectile::StaticClass(), m_SpellOwner->GetCastStartLocation(), FRotator::ZeroRotator, spawnParams);

	if (projectile == nullptr)
	{
		return;
	}

	projectile->Init(m_DecoratedSelf.GetInterface(), m_SpellData.Get());
	projectile->AddIgnoreActor(owner);
	m_DecoratedSelf->DecorateProjectile(projectile);

	if (m_SpellNiagara.IsValid() == true)
	{
		projectile->SetNiagara(m_SpellNiagara.Get());
	}

	projectile->SetIsActive(true);
	projectile->ApplyForce(direction);
}

void UProjectileSpell::ProcessOverlapDamage(int& damage)
{
	damage += m_SpellData->HitDamage;
}

void UProjectileSpell::ProcessHitDamage(int& damage, FVector targetLocation, FVector hitLocation)
{
	damage += m_SpellData->HitDamage;
}

bool UProjectileSpell::DealDamage(AActor* hitActor, int damage)
{
	if (ISusceptible* target = Cast<ISusceptible>(hitActor))
	{
		target->Damage(damage, true);
		return target->IsDead();
	}

	return true;
}

void UProjectileSpell::DecorateProjectile(IProjectile* projectile)
{
	projectile->SetDecoratedSelf(projectile);
}

USpellData* UProjectileSpell::GetSpellData()
{
	if (m_SpellData.IsValid() == false)
	{
		LOG_ERROR("[PROJECTILE SPELL] Tried to GetSpellData for ProjectileSpell but SpellData is invalid");
		return nullptr;
	}

	return m_SpellData.Get();
}
