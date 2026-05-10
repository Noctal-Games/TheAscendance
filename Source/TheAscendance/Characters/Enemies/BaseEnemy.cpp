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
#include "TheAscendance/AI/Combat/Components/CombatAIComponent.h"	
#include "TheAscendance/AI/Actions/Attacks/Structs/AttackData.h"

#include "Components/CapsuleComponent.h"

ABaseEnemy::ABaseEnemy() : ABaseCharacter()
{
	GetCapsuleComponent()->SetVisibility(true);
	GetCapsuleComponent()->bHiddenInGame = false;

	m_CombatAgent = CreateDefaultSubobject<UCombatAIComponent>(TEXT("Combat Agent Component"));
	checkf(m_CombatAgent, TEXT("Combat Agent Component failed to initialise"));
}

void ABaseEnemy::Init(const UEnemyData* data)
{
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
	}

	TWeakObjectPtr<ABaseEnemy> weakThis(this);
	TArray<FEnemyAbilityData> abilities = combatSettings.Abilities;

	UStreamableFunctionLibrary::RequestAsyncLoad(assetPaths, [weakThis, abilities, data]()
		{
			if (weakThis.IsValid() == false)
			{
				LOG_WARNING("[BASE ENEMY] BaseEnemy was destroyed before assets finished loading, cancelling setup");
				return;
			}

			weakThis->SetSkeletalMesh();
			weakThis->InitCombatAIComponent(abilities, data->ClassData, data->CombatSettings.GoalWeights);
		});
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
	//if (m_Agent == nullptr)
	//{
	//	return;
	//}

	//return m_Agent->SetLocationToInvestigate(location);
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
	//if (route == nullptr || m_Agent == nullptr)
	//{
	//	return;
	//}

	//m_Agent->SetWaypointRoute(route);
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
	return false;// m_Agent->IsSoundHeard(soundWeight);
}

bool ABaseEnemy::IsInCombat() const
{
	//if (m_Agent == nullptr)
	//{
	//	return true;
	//}

	return false;// m_Agent->IsInCombat();
}

void ABaseEnemy::StartAbility(const FGameplayTag& abilityTag)
{
	if (m_AbilityComponent == nullptr)
	{
		LOG_ERROR("[BASE ENEMY] AbilityComponent is invalid")
		return;
	}

	m_AbilityComponent->StartAbility(abilityTag);
}

bool ABaseEnemy::IsAbilityOnCooldown(const FGameplayTag& abilityTag)
{
	if (m_AbilityComponent == nullptr)
	{
		LOG_ERROR("[BASE ENEMY] AbilityComponent is invalid")
		return false;
	}

	return m_AbilityComponent->IsAbilityOnCooldown(abilityTag);
}

void ABaseEnemy::BeginPlay()
{
	ABaseCharacter::BeginPlay();

	OwnedTags.AddTag(CHARACTER_ENEMY);

	m_Controller = Cast<ATAAIController>(GetController());
}

void ABaseEnemy::InitCombatAIComponent(const TArray<FEnemyAbilityData>& abilities, const UEnemyClassData* classData, const TMap<EAbilityGoal, float>& goalWeights)
{
	if (m_CombatAgent == nullptr)
	{
		LOG_ERROR("[BASE ENEMY] Tried to InitCombatAIComponent with invalid CombatAgent");
		return;
	}

	if(classData == nullptr)
	{
		LOG_ERROR("[BASE ENEMY] Tried to InitCombatAIComponent with invalid EnemyClassData");
		return;
	}

	FLoadedCombatSettings combatSettings;

	for (const FEnemyAbilityData& abilityData : abilities)
	{
		FLoadedAbilityData loadedData = ProcessLoadedAbilityData(abilityData);
		
		if (loadedData.AbilityTag.IsValid() == false)
		{
			continue;
		}

		combatSettings.Abilities.Add(MoveTemp(loadedData));
	}

	combatSettings.GoalWeights = goalWeights;
	combatSettings.MaxEngagementRange = classData->MaxEngagementRange;
	combatSettings.PreferredEngagementRange = classData->PreferredEngagementRange;
	combatSettings.EngagementRangeTolerance = classData->EngagementRangeTolerance;

	m_CombatAgent->Init(combatSettings, m_AbilityComponent);
}

FLoadedAbilityData ABaseEnemy::ProcessLoadedAbilityData(const FEnemyAbilityData& abilityData)
{
	FLoadedAbilityData loadedData;

	if (UAbilityData* ability = abilityData.AbilityData.Get())
	{
		if (m_AbilityComponent == nullptr)
		{
			LOG_ERROR("[BASE ENEMY] Tried to ProcessAbilityData with invalid AbilityComponent");
			return loadedData;
		}

		if (m_AbilityComponent->AddAbilityFromData(ability) == false)
		{
			LOG_ERROR("[BASE ENEMY] Failed to add ability from data with tag %s", *ability->AbilityTag.ToString());
			return loadedData;
		}

		loadedData.AbilityTag = ability->AbilityTag;
	}
	else
	{
		LOG_ERROR("[BASE ENEMY] Tried to ProcessAbilityData with invalid AbilityData");
		return loadedData;
	}

	loadedData.Weight = abilityData.Weight;
	loadedData.Goals = abilityData.Goals;

	return loadedData;
}
