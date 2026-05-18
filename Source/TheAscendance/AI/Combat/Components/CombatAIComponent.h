// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TheAscendance/AI/Enums/State.h"
#include "TheAscendance/Characters/Enemies/Structs/EnemyData.h"
#include "TheAscendance/AI/Combat/Structs/CombatContext.h"
#include "TheAscendance/AI/Actions/Attacks/Structs/AttackData.h"
#include "TheAscendance/AI/Perception/Structs/PerceptionContext.h"
#include "CombatAIComponent.generated.h"

class UAbstractCombatState;
class UAbilityComponent;
class ABaseEnemy;
class ATAAIController;
class UPerceptionComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEASCENDANCE_API UCombatAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatAIComponent();

	void Init(UPerceptionComponent* perceptionComponent, const FLoadedCombatSettings& combatSettings, UAbilityComponent* abilityComponent);
	void SetState(ECombatState newState);

	void UseAbility();
	void SetIsCombatLocked(bool val);

	void SetIsAIMovementPaused(bool val);
	void SetFocus(AActor* target);

	void LookAtTarget();
	void RequestWaitForState();
	void NotifyStateFinished();

	void SetDestination(const FVector& destination);
	void GetCombatRangeValues(float& PreferredEngagementRange, float& MaxEngagementRange, float& EngagementRangeTolerance);

	float GetAggression();

	ABaseEnemy* GetEnemyOwner();
	AActor* GetCurrentTarget();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	float ScoreAttack() const;
	float ScoreSupport() const;
	float ScoreSelf() const;
	float ScoreRetreat() const;
	float ScoreStrafe() const;

private:
	void EvaluateCombat();
	void HandleAbilityFinished();
	bool TryConsumeReaction();
	void SmoothMovementIntent(float deltaTime);
	void ApplyMovement(float deltaTime);

public:
	UPROPERTY()
	FCombatContext CombatContext;
	UPROPERTY()
	FLoadedCombatSettings CombatSettings;
	UPROPERTY()
	FPerceptionContext PerceptionContext;
	UPROPERTY()
	FBehaviourSettings BehaviourSettings;

protected:
	friend class URetreatCombatState;
	friend class UStrafeCombatState;
	friend class UApproachCombatState;

	FVector m_MovementIntent = FVector::ZeroVector;

private:
	UPROPERTY()
	TWeakObjectPtr<ABaseEnemy> m_Owner = nullptr;
	UPROPERTY()
	TWeakObjectPtr<ATAAIController> m_Controller = nullptr;
	UPROPERTY()
	TWeakObjectPtr<UPerceptionComponent> m_PerceptionComponent = nullptr;

	UPROPERTY()
	TMap<ECombatState, TObjectPtr<UAbstractCombatState>> m_CombatStates;
	ECombatState m_CurrentCombatState = ECombatState::MAX;

	FVector m_SmoothedMovement = FVector::ZeroVector;

	float m_LastReactionTime = 0.0f;
	float m_CurrentReactionTime = 0.0f;

	bool m_IsCombatLocked = false;

	bool m_WaitForState = false;
	bool m_StateFinished = false;
};
