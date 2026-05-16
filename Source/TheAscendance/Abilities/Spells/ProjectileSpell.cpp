// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileSpell.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/StreamableFunctionLibrary.h"
#include "Structs/SpellData.h"
#include "TheAscendance/Actors/Projectile/Interfaces/Projectile.h"
#include "TheAscendance/Core/AbilityHelpers.h"
#include "TheAscendance/Abilities/Components/AbilityComponent.h"

void UProjectileSpell::Init(UAbilityComponent* ownerComponent, UAbilityData* abilityData)
{
	UBaseSpell::Init(ownerComponent, abilityData);

	if (m_AbilityData == nullptr || m_OwnerComponent == nullptr)
	{
		return;
	}

	if (UProjectileSpellData* data = Cast<UProjectileSpellData>(m_AbilityData))
	{
		m_SpellData = data;

		m_ProjectileNiagara = m_SpellData->ProjectileNiagara;

		if (m_ProjectileNiagara.IsNull() == true)
		{
			LOG_ERROR("[PROJECTILE SPELL] Projectile Niagara is invalid");
			return;
		}

		UStreamableFunctionLibrary::RequestAsyncLoad(m_ProjectileNiagara.ToSoftObjectPath());
	}
	else
	{
		LOG_ERROR("[PROJECTILE SPELL] Tried to Init ProjectileSpellBase with invalid SpellData");
	}
}

void UProjectileSpell::TriggerAbility()
{
	if(m_SpellData == nullptr)
	{
		LOG_ERROR("[PROJECTILE SPELL] Tried to Execute ProjectileSpell with invalid SpellData");
		return;
	}

	if (m_OwnerComponent == nullptr)
	{
		LOG_ERROR("[PROJECTILE SPELL] Tried to Execute ProjectileSpell with invalid Owner");
		return;
	}

	if(m_DecoratedSelf == nullptr)
	{
		LOG_ERROR("[PROJECTILE SPELL] Tried to Execute ProjectileSpell with invalid DecoratedSelf");
		return;
	}

	if (IProjectile* projectile = UAbilityHelpers::SpawnProjectile(m_DecoratedSelf.GetInterface(), m_OwnerComponent->GetCastLocation(), &m_SpellData->ProjectileModifiers))
	{
		projectile->AddIgnoreActor(GetAbilityOwner());
		projectile->SetIsActive(true);

		if (m_ProjectileNiagara.IsValid() == true)
		{
			projectile->SetNiagara(m_ProjectileNiagara.Get());
		}

		projectile->ApplyForce(m_OwnerComponent->GetCastForward());
	}
	else
	{
		LOG_ERROR("[PROJECTILE SPELL] Failed to spawn Projectile");
	}

	UBaseSpell::TriggerAbility();
}

void UProjectileSpell::ProcessOverlapDamage(int& damage)
{
	damage += m_SpellData->HitDamage;
}

void UProjectileSpell::ProcessHitDamage(int& damage, const FVector& targetLocation, const FVector& hitLocation)
{
	damage += m_SpellData->HitDamage;
}

//void UProjectileSpell::LoadHitNiagara()
//{
//	if(m_SpellData.IsValid() == false)
//	{
//		LOG_ERROR("[PROJECTILE SPELL] Tried to Load HitNiagara for ProjectileSpell with invalid SpellData");
//		return;
//	}
//
//	m_HitNiagara = m_SpellData->SpellHitNiagara;
//
//	if (m_HitNiagara.IsNull() == true)
//	{
//		LOG_WARNING("[PROJECTILE SPELL] Tried to Load HitNiagara for ProjectileSpell with invalid SpellHitNiagara");
//		return;
//	}
//
//	UCoreFunctionLibrary::RequestAsyncLoad(m_HitNiagara.ToSoftObjectPath());
//}

//void UProjectileSpell::Fire(const FVector& direction)
//{
//	if (m_DecoratedSelf == nullptr)
//	{
//		LOG_ERROR("[PROJECTILE SPELL] Tried to Fire Projectile with invalid DecoratedSelf");
//		return;
//	}
//
//	if (m_HitNiagara.IsValid() == false)
//	{
//		m_DecoratedSelf->LoadHitNiagara();
//	}
//
//	AActor* owner = m_SpellOwner->GetActor();
//
//	FActorSpawnParameters spawnParams;
//	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
//	ABaseProjectile* projectile = owner->GetWorld()->SpawnActor<ABaseProjectile>(ABaseProjectile::StaticClass(), m_SpellOwner->GetCastStartLocation(), FRotator::ZeroRotator, spawnParams);
//
//	if (projectile == nullptr)
//	{
//		return;
//	}
//
//	projectile->Init(m_DecoratedSelf.GetInterface(), m_SpellData.Get());
//	projectile->AddIgnoreActor(owner);
//	m_DecoratedSelf->DecorateProjectile(projectile);
//
//	if (m_SpellNiagara.IsValid() == true)
//	{
//		projectile->SetNiagara(m_SpellNiagara.Get());
//	}
//
//	projectile->SetIsActive(true);
//	projectile->ApplyForce(direction);
//}

//void UProjectileSpell::DecorateProjectile(IProjectile* projectile)
//{
//	projectile->SetDecoratedSelf(projectile);
//}

//USpellData* UProjectileSpell::GetSpellData()
//{
//	if (m_SpellData.IsValid() == false)
//	{
//		LOG_ERROR("[PROJECTILE SPELL] Tried to GetSpellData for ProjectileSpell but SpellData is invalid");
//		return nullptr;
//	}
//
//	return m_SpellData.Get();
//}
