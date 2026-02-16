// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Characters/Components/CharacterStatsComponent.h"
#include "TheAscendance/Characters/CharacterGameplayTags.h"
#include "TheAscendance/AI/TAAIController.h"
#include "TheAscendance/AI/Components/HSMAgentComponent.h"
#include "TheAscendance/Characters/Components/LoadoutComponent.h"
#include "TheAscendance/Game/Subsystems/GameEventSubsystem.h"
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
		LOG_ERROR("[BASE ENEMY] Tried to Init BaseEnemy with invalid EnemyTableData")
		return;
	}

	m_EnemyID = data->EnemyID;

	TArray<FSoftObjectPath> assetPaths;
	m_SkeletalMesh = data->EnemyMesh;

	if (m_SkeletalMesh.IsNull() == false)
	{
		assetPaths.Add(m_SkeletalMesh.ToSoftObjectPath());
	}
	else
	{
		LOG_ERROR("[BASE ENEMY] Tried to Init BaseEnemy with invalid EnemyMesh");
	}

	m_AnimationBP = data->EnemyAnimationBP;

	if (m_AnimationBP.IsNull() == false)
	{
		assetPaths.Add(m_AnimationBP.ToSoftObjectPath());
	}
	else
	{
		LOG_ERROR("[BASE ENEMY]Tried to Init BaseEnemy with invalid EnemyAnimationBP");
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

	for (const FGameplayTag& immunity : stats.EffectImmunities)
	{
		AddImmunity(immunity);
	}

	for (const FGameplayTag& resistance : stats.EffectResistances)
	{
		AddResistance(resistance);
	}

	const FEnemyEquipmentData& equipment = data->EnemyData.EnemyEquipment;

	TArray<FGameplayTag> spells;
	spells.Add(equipment.MainHandSpells.PrimarySpell);
	spells.Add(equipment.MainHandSpells.SecondarySpell);
	spells.Add(equipment.OffHandSpells.PrimarySpell);
	spells.Add(equipment.OffHandSpells.SecondarySpell);

	m_LoadoutComponent->SetSpells(spells);

	for (const FLoadoutSlotData& loadoutData : equipment.LoadoutData)
	{
		m_LoadoutComponent->EquipItem(loadoutData.EquippedPart, loadoutData.ItemTag);
	}

	if(m_Agent = NewObject<UHSMAgentComponent>(this, "HSM_AGENT"))
	{
		m_Agent->RegisterComponent();
		m_Agent->InitStats(stats.SightStrength, stats.HearingStrength, stats.PreferredRange, stats.PreferredRangeTolerance, stats.ReactionTimeMinimum, stats.ReactionTimeMaximum);
		m_Agent->InitMeleeAttacks(data->EnemyData.AttackSet);
		m_Agent->Init(this);
	}
	else
	{
		LOG_ERROR("[BASE ENEMY] Failed to initalise HSM_Agent");
	}
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
			LOG_ERROR("[BASE ENEMY] BaseEnemy AnimationBP is invalid");
		}

		FBoxSphereBounds bounds = GetMesh()->Bounds;
		FVector extent = bounds.BoxExtent;

		float radius = FMath::Max(extent.X, extent.Y);
		float halfHeight = extent.Z;

		GetCapsuleComponent()->SetCapsuleSize(radius, halfHeight);
		GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -halfHeight));
	}
	else
	{
		LOG_ERROR("[BASE ENEMY] BaseEnemy SkeletalMesh is invalid");
	}
}

void ABaseEnemy::Damage(int amount)
{
	ABaseCharacter::Damage(amount);

	if (IsDead() == false)
	{
		return;
	}

	if (UGameEventSubsystem* gameEvent = GetWorld()->GetGameInstance()->GetSubsystem<UGameEventSubsystem>())
	{
		gameEvent->NotifyEnemyKilled(m_EnemyID);
	}
}

void ABaseEnemy::SetDestination(const FVector& destination)
{
	if (m_Controller.IsValid() == false)
	{
		LOG_ERROR("[BASE ENEMY] Tried to set destination with invalid controller");
		return;
	}

	m_Controller->SetDestination(destination);
}

void ABaseEnemy::SetLocationToInvestigate(const FVector& location)
{
	if (m_Agent == nullptr)
	{
		return;
	}

	return m_Agent->SetLocationToInvestigate(location);
}

bool ABaseEnemy::HasPath() const
{
	if (m_Controller.IsValid() == false)
	{
		LOG_ERROR("[BASE ENEMY] Tried to get HasPath with invalid Controller");
		return true;
	}

	return m_Controller->HasPath();
}

void ABaseEnemy::SetWaypointRoute(AWaypointRoute* route)
{
	if (route == nullptr || m_Agent == nullptr)
	{
		return;
	}

	m_Agent->SetWaypointRoute(route);
}

void ABaseEnemy::SetFocus(AActor* target)
{
	if (m_Controller.IsValid() == false)
	{
		LOG_ERROR("[BASE ENEMY] Tried to set focus with invalid controller");
		return;
	}

	m_Controller->SetFocus(target);
}

void ABaseEnemy::ClearFocus()
{
	if (m_Controller.IsValid() == false)
	{
		LOG_ERROR("[BASE ENEMY] Tried to clear focus with invalid controller");
		return;
	}

	m_Controller->ClearFocus(EAIFocusPriority::Gameplay);
}

bool ABaseEnemy::IsSoundHeard(float soundWeight) const
{
	if (m_Agent == nullptr)
	{
		return true;
	}

	return m_Agent->IsSoundHeard(soundWeight);
}

bool ABaseEnemy::IsInCombat() const
{
	if (m_Agent == nullptr)
	{
		return true;
	}

	return m_Agent->IsInCombat();
}

void ABaseEnemy::BeginPlay()
{
	ABaseCharacter::BeginPlay();

	OwnedTags.AddTag(CHARACTER_ENEMY);

	m_Controller = Cast<ATAAIController>(GetController());
}
