// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Characters/Components/CharacterStatsComponent.h"
#include "Structs/EnemyData.h"

#include "Components/CapsuleComponent.h"

ABaseEnemy::ABaseEnemy() : ABaseCharacter()
{
	GetCapsuleComponent()->SetVisibility(true);
	GetCapsuleComponent()->bHiddenInGame = false;
}

void ABaseEnemy::Init(FEnemyTableData* data)
{
	m_AnimTest = true;

	if (data == nullptr)
	{
		LOG_ERROR("Tried to Init BaseEnemy with invalid EnemyTableData")
		return;
	}

	TArray<FSoftObjectPath> assetPaths;

	m_SkeletalMesh = data->EnemyMesh;

	if (m_SkeletalMesh.IsNull() == false)
	{
		assetPaths.Add(m_SkeletalMesh.ToSoftObjectPath());
	}
	else
	{
		LOG_ERROR("Tried to Init BaseEnemy with invalid EnemyMesh");
	}

	m_AnimationBP = data->EnemyAnimationBP;

	if (m_AnimationBP.IsNull() == false)
	{
		assetPaths.Add(m_AnimationBP.ToSoftObjectPath());
	}
	else
	{
		LOG_ERROR("Tried to Init BaseEnemy with invalid EnemyAnimationBP");
	}

	UCoreFunctionLibrary::RequestAsyncLoad(assetPaths, [this]() { SetSkeletalMesh(); });

	const FEnemyStats& stats = data->EnemyData.EnemyStats;

	m_CharacterStatsComponent->SetStat(ECharacterStat::HEALTH, stats.Health);
	m_CharacterStatsComponent->SetStat(ECharacterStat::WALK_SPEED, stats.WalkSpeed);
	m_CharacterStatsComponent->SetStat(ECharacterStat::SPRINT_SPEED_BONUS, stats.SprintSpeedBonus);
	//m_CharacterStatsComponent->SetStat(ECharacterStat::MAGIC_ATTACK, stats->MagicAttack);
	//m_CharacterStatsComponent->SetStat(ECharacterStat::MAGIC_RESISTANCE, stats->MagicResistance);
	//m_CharacterStatsComponent->SetStat(ECharacterStat::PHYSICAL_ATTACK, stats->PhysicalAttack);
	//m_CharacterStatsComponent->SetStat(ECharacterStat::PHYSICAL_RESISTANCE, stats->PhysicalResistance);
	m_CharacterStatsComponent->SetStat(ECharacterStat::MANA, 999999);
	m_CharacterStatsComponent->SetStat(ECharacterStat::STAMINA, 999999);

	const FEnemyEquipmentData& equipment = data->EnemyData.EnemyEquipment;

	TArray<int> spells;
	spells.Add(equipment.MainHandSpells.PrimarySpell);
	spells.Add(equipment.MainHandSpells.SecondarySpell);
	spells.Add(equipment.OffHandSpells.PrimarySpell);
	spells.Add(equipment.OffHandSpells.SecondarySpell);

	if (equipment.LoadoutData.Num() == 0)
	{                               
		return;
	}
	
	//Equip items
}

void ABaseEnemy::SetSkeletalMesh()
{
	if (m_SkeletalMesh.IsValid() == true)
	{
		GetMesh()->SetSkeletalMesh(m_SkeletalMesh.Get());

		if (m_AnimationBP.IsValid() == true)
		{
			GetMesh()->SetAnimInstanceClass(m_AnimationBP.Get());
		}
		else
		{
			LOG_ERROR("BaseEnemy AnimationBP is invalid");
		}

		FBoxSphereBounds bounds = GetMesh()->Bounds;
		FVector extent = bounds.BoxExtent;

		float radius = FMath::Max(extent.X, extent.Y);
		float halfHeight = extent.Z;

		//GetCapsuleComponent()->SetCapsuleSize(radius, halfHeight);
		//GetCapsuleComponent()->SetRelativeLocation(bounds.Origin);
	}
	else
	{
		LOG_ERROR("BaseEnemy SkeletalMesh is invalid");
	}
}
