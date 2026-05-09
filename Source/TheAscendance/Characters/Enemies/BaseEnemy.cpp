// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/StreamableFunctionLibrary.h"
#include "TheAscendance/Characters/Components/CharacterStatsComponent.h"
#include "TheAscendance/Characters/CharacterGameplayTags.h"
#include "TheAscendance/AI/TAAIController.h"
#include "TheAscendance/AI/Components/HSMAgentComponent.h"
#include "TheAscendance/Characters/Components/LoadoutComponent.h"
#include "TheAscendance/Game/Subsystems/GameEventSubsystem.h"
#include "Structs/EnemyData.h"
#include "TheAscendance/Abilities/Components/AbilityComponent.h"

#include "Components/CapsuleComponent.h"

ABaseEnemy::ABaseEnemy() : ABaseCharacter()
{
	GetCapsuleComponent()->SetVisibility(true);
	GetCapsuleComponent()->bHiddenInGame = false;
}

void ABaseEnemy::Init(const UEnemyData* data)
{
	m_AnimTest = true;

	if (data == nullptr)
	{
		LOG_ERROR("[BASE ENEMY] Tried to Init BaseEnemy with invalid EnemyTableData")
		return;
	}

	m_EnemyTag = data->EnemyTag;

	const FEnemyStats& stats = data->Stats;
	m_CharacterStatsComponent->SetStat(ECharacterStat::HEALTH, stats.Health);
	m_CharacterStatsComponent->SetStat(ECharacterStat::MAGIC_ATTACK, stats.MagicAttack);
	m_CharacterStatsComponent->SetStat(ECharacterStat::MAGIC_RESISTANCE, stats.MagicResistance);
	m_CharacterStatsComponent->SetStat(ECharacterStat::PHYSICAL_ATTACK, stats.PhysicalAttack);
	m_CharacterStatsComponent->SetStat(ECharacterStat::PHYSICAL_RESISTANCE, stats.PhysicalResistance);
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

	const FEnemyMovementSettings& movementStats = data->MovementSettings;
	m_CharacterStatsComponent->SetStat(ECharacterStat::WALK_SPEED, movementStats.WalkSpeed);
	m_CharacterStatsComponent->SetStat(ECharacterStat::SPRINT_SPEED_BONUS, movementStats.SprintSpeed - movementStats.WalkSpeed);

	TArray<FSoftObjectPath> assetPaths;
	m_SkeletalMesh = data->EnemyMesh;

	if (m_SkeletalMesh.IsNull() == false)
	{
		assetPaths.AddUnique(m_SkeletalMesh.ToSoftObjectPath());
	}
	else
	{
		LOG_ERROR("[BASE ENEMY] Tried to Init BaseEnemy with invalid EnemyMesh");
	}

	m_AnimationBP = data->EnemyAnimationBP;

	if (m_AnimationBP.IsNull() == false)
	{
		assetPaths.AddUnique(m_AnimationBP.ToSoftObjectPath());
	}
	else
	{
		LOG_ERROR("[BASE ENEMY]Tried to Init BaseEnemy with invalid EnemyAnimationBP");
	}

	const FCombatSettings& combatSettings = data->CombatSettings;

	for (const FEnemyAbilityData& ability : combatSettings.Abilities)
	{
		if (ability.AbilityData.IsNull() == false)
		{
			assetPaths.AddUnique(ability.AbilityData.ToSoftObjectPath());
		}

		if (ability.TelegraphMontage.IsNull() == false)
		{
			assetPaths.AddUnique(ability.TelegraphMontage.ToSoftObjectPath());
		}
	}

	TWeakObjectPtr<ABaseEnemy> weakThis(this);
	TArray<FEnemyAbilityData> abilities = combatSettings.Abilities;

	UStreamableFunctionLibrary::RequestAsyncLoad(assetPaths, [weakThis, abilities]()
		{
			if (weakThis.IsValid() == false)
			{
				return;
			}

			weakThis->SetSkeletalMesh();

			for (const FEnemyAbilityData& ability : abilities)
			{
				weakThis->InitAbilityData(ability);
			}
		});

	if(m_Agent = NewObject<UHSMAgentComponent>(this, "HSM_AGENT"))
	{
		m_Agent->RegisterComponent();
		m_Agent->Init(this, data->ClassData, data->BehaviourSettings, data->PerceptionSettings);
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

void ABaseEnemy::Damage(int amount, bool triggerOnHit)
{
	ABaseCharacter::Damage(amount, triggerOnHit);

	if (IsDead() == false)
	{
		return;
	}

	if (UGameEventSubsystem* gameEvent = GetWorld()->GetGameInstance()->GetSubsystem<UGameEventSubsystem>())
	{
		gameEvent->NotifyEnemyKilled(m_EnemyTag);
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

void ABaseEnemy::InitAbilityData(const FEnemyAbilityData& abilityData)
{
	FEnemyLoadedAbilityData loadedData;

	if(UAbilityData* ability = abilityData.AbilityData.Get())
	{
		if (m_AbilityComponent == nullptr)
		{
			LOG_ERROR("[BASE ENEMY] Tried to init ability data with invalid AbilityComponent");
			return;
		}

		if(m_AbilityComponent->AddAbilityFromData(ability) == false)
		{
			LOG_ERROR("[BASE ENEMY] Failed to add ability from data with tag %s", *ability->AbilityTag.ToString());
			return;
		}

		loadedData.AbilityTag = ability->AbilityTag;
	}
	else
	{
		LOG_ERROR("[BASE ENEMY] Tried to init ability data with invalid AbilityData");
		return;
	}

	loadedData.Weight = abilityData.Weight;
	loadedData.Goals = abilityData.Goals;

	//Failure isn't logged as a telegraph montage isn't needed. Checks can be set in future as abilities are setup
	if(UAnimMontage* montage = abilityData.TelegraphMontage.Get())
	{
		loadedData.TelegraphMontage = montage;
	}

	//m_AbilityData.Add(MoveTemp(loadedData));
}
