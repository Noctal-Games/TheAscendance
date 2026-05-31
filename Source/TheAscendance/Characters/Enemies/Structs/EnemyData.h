// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UObject/AssetRegistryTagsContext.h"
#include "TheAscendance/Characters/Structs/LoadoutSlotData.h"
#include "TheAscendance/AI/Actions/Attacks/Structs/AttackData.h"
#include "GameplayTags.h"
#include "EnemyData.generated.h"

class UStateTree;

USTRUCT(BlueprintType)
struct FFloatRangeCustom
{
	GENERATED_BODY()


	float GetRandomValue() const
	{
		return FMath::FRandRange(Min, Max);
	}

	UPROPERTY(EditDefaultsOnly)
	float Min = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float Max = 0.0f;
};

UCLASS(BlueprintType)
class THEASCENDANCE_API UEnemyClassData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStateTree> CombatStateTree = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Checks whether the enemy has abilities that fulfil these goals. A warning will be logged otherwise"))
	TArray<EAbilityGoal> ExpectedAbilityGoals;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The range at which the enemy prefers to engage, the enemy will try to stay at this distance where possible"))
	float PreferredEngagementRange = 0.0f;
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The maximum range at which the enemy can engage"))
	float MaxEngagementRange = 0.0f;
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Lower value means that the target is less likely to prioritise moving to their preferred range. An aggressive enemy might not care about moving away from the player if they are closer than their preferred range, for example.", ClampMin = 0.1f, ClampMax = 1.0f))
	float EngagementRangeTolerance = 0.1f;
};

USTRUCT(BlueprintType)
struct FEnemyStats
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float Health = 0.0f;

	UPROPERTY(EditAnywhere)
	float PhysicalAttack = 0.0f;

	UPROPERTY(EditAnywhere)
	float MagicAttack = 0.0f;

	UPROPERTY(EditAnywhere)
	float PhysicalResistance = 0.0f;

	UPROPERTY(EditAnywhere)
	float MagicResistance = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> EffectImmunities;
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameplayTag> EffectResistances;
};

USTRUCT(BlueprintType)
struct FPerceptionSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, meta = (Category = "Vision"))
	float SightMaxDistance = 5000.0f;
	UPROPERTY(EditDefaultsOnly, meta = (Category = "Vision", ToolTip = "How strongly distance affects vision as it reaches SightMaxDistance"))
	TObjectPtr<UCurveFloat> DistanceVisibilityCurve = nullptr;
	UPROPERTY(EditDefaultsOnly, meta = (Category = "Vision", ToolTip = "Higher value for stronger vision", ClampMin = 0.0f, ClampMax = 1.0f))
	float SightStrength = 1.0f;
	UPROPERTY(EditDefaultsOnly, meta = (Category = "Vision", ToolTip = "Higher value for stronger vision", ClampMin = 0, ClampMax = 360))
	int FOV = 90;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "Higher value for stronger hearing", ClampMin = 0.0f, ClampMax = 1.0f))
	float HearingStrength = 1.0f;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The threshold in which the enemy detects the target", ClampMin = 0.0f, ClampMax = 1.0f))
	float DetectionThreshold = 1.0f;
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "How fast the enemy can detect a target. Added per tick with Line of Sight.", ClampMin = 0.1f, ClampMax = 2.0f))
	float DetectionGainRate = 0.1f;
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "How fast the enemy can lose track of a target. Negated per tick without Line of Sight", ClampMin = 0.1f, ClampMax = 1.0f))
	float DetectionLossRate = 0.1f;
};

USTRUCT(BlueprintType)
struct FBehaviourSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "An Aggression of 1.0 will mean the enemy will always advance on the target. Also means damage abilities are prioritised over self healing, for example.", ClampMin = 0.1f, ClampMax = 1.0f))
	float Aggression = 0.0f;
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "An Selflessness of 1.0 will mean the enemy will always prioritise abilites that support allies when possible.", ClampMin = 0.1f, ClampMax = 1.0f))
	float Selflessness = 0.0f;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "How often the agent reacts to the information it has and makes a decision."))
	FFloatRangeCustom ReactionTime;

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "The duration that the enemy will remember where the target was when detection was lost"))
	float DetectionMemoryDuration = 1.0f;
	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "A random modifier for weighted decisions, to add variety to behaviour"))
	FFloatRangeCustom DecisionRandomness;
};

USTRUCT(BlueprintType)
struct FCombatSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (ToolTip = "A map of enemy abilities and their weights. Higher weighted abilities will have a higher base priority."))
	TArray<FEnemyAbilityData> Abilities;
	UPROPERTY(EditDefaultsOnly)
	TMap<EAbilityGoal, float> GoalWeights;
};

USTRUCT(BlueprintType)
struct FEnemyMovementSettings
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float WalkSpeed = 0.0f;
	UPROPERTY(EditDefaultsOnly)
	float SprintSpeed = 0.0f;
	//UPROPERTY(EditDefaultsOnly)
	//float ChargeSpeed = 0.0f;
};

UCLASS(BlueprintType)
class THEASCENDANCE_API UEnemyData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual void GetAssetRegistryTags(FAssetRegistryTagsContext context) const override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Enemy", AssetRegistrySearchable))
	FGameplayTag EnemyTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<class USkeletalMesh> EnemyMesh = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<class UAnimInstance> EnemyAnimationBP = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UEnemyClassData> ClassData = nullptr;
	UPROPERTY(EditDefaultsOnly)
	FEnemyStats Stats;
	UPROPERTY(EditDefaultsOnly)
	FPerceptionSettings PerceptionSettings;
	UPROPERTY(EditDefaultsOnly)
	FBehaviourSettings BehaviourSettings;
	UPROPERTY(EditDefaultsOnly)
	FCombatSettings CombatSettings;
	UPROPERTY(EditDefaultsOnly)
	FEnemyMovementSettings MovementSettings;
};
