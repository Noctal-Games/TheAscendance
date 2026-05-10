// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TheAscendance/AI/Enums/State.h"
#include "TheAscendance/Characters/Enemies/Structs/EnemyData.h"
#include "TheAscendance/AI/Combat/Structs/CombatContext.h"
#include "TheAscendance/AI/Actions/Attacks/Structs/AttackData.h"
#include "CombatAIComponent.generated.h"

class UAbstractCombatState;
class UAbilityComponent;
class ABaseEnemy;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEASCENDANCE_API UCombatAIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatAIComponent();

	void Init(const FLoadedCombatSettings& combatSettings, UAbilityComponent* abilityComponent);
	void SetState(ECombatState newState);

	void UseAbility();
	void SetIsCombatLocked(bool val);

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

public:
	UPROPERTY()
	FCombatContext CombatContext;
	UPROPERTY()
	FLoadedCombatSettings CombatSettings;

private:
	UPROPERTY()
	TWeakObjectPtr<ABaseEnemy> m_Owner = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FBehaviourSettings m_BehaviourSettings;

	UPROPERTY()
	TMap<ECombatState, TObjectPtr<UAbstractCombatState>> m_CombatStates;
	ECombatState m_CurrentCombatState = ECombatState::MAX;

	float m_DecisionTimer = 0.0f;

	bool m_IsCombatLocked = false;
};
