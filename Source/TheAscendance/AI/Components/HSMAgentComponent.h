// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TheAscendance/AI/Enums/State.h"
#include "TheAscendance/AI/Actions/Attacks/Enums/AttackType.h"
#include "TheAscendance/AI/Actions/Attacks/Structs/AttackData.h"
#include "TheAscendance/Characters/Enemies/Structs/EnemyData.h"
#include "HSMAgentComponent.generated.h"

class ABaseEnemy;
class APlayerCharacter;
class UAbstractState;
class AWaypointRoute;
class USightSensorComponent;
class UEnemyClassData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEASCENDANCE_API UHSMAgentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHSMAgentComponent();

	void Init(ABaseEnemy* owner);
	void InitSettings(UEnemyClassData* classData, const FBehaviourSettings& behaviourSettings, const FPerceptionSettings& perceptionSettings);
	//void InitMeleeAttacks(const FAttackSetData& attackSet /*const TMap<EMeleeAttackType, TWeakObjectPtr<FAttackData>>& meleeAttacks*/);

	//const FAttackSetData* GetAttackSetData() const;
	//FAttackData* GetMeleeAttackData(EMeleeAttackType attackType) const;

	void SetState(EState newState);
	void SetCombatState(ECombatState combatState);

	void SetDestination(const FVector& destination);

	void SetLocationToInvestigate(const FVector& location);
	FVector GetLocationToInvestigate();

	ABaseEnemy* GetAgentOwner() const;
	APlayerCharacter* GetTargetPlayer() const;

	bool HasPath() const;

	bool IsTargetInActionableRange(const FVector& target) const;
	void GetPreferredDistanceValues(float& preferredDistanceFromTarget, float& preferredDistanceTolerance) const;

	void SetFocus(AActor* target);
	void ClearFocus();

	//Temp until better setup
	void SetWaypointRoute(AWaypointRoute* route);
	AWaypointRoute* GetWaypointRoute() const;

	float GetVisionStrength() const;
	float GetHearingStrength() const;

	float GetRandomCombatReactionTime() const;

	bool HasLineOfSight() const;
	void SetHasLineOfSight(bool hasLineOfSight);

	bool IsSoundHeard(float soundWeight) const;
	bool IsInCombat() const;

	bool IsTargetTooClose(const FVector& target) const;
	bool IsTargetTooFar(const FVector& target) const;

	void AddAbility(const FEnemyLoadedAbilityData& abilityData);

	// Called every frame
	virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<USightSensorComponent> m_SightSensor = nullptr;

	UPROPERTY()
	TWeakObjectPtr<ABaseEnemy> m_Owner = nullptr;
	UPROPERTY()
	TWeakObjectPtr<APlayerCharacter> m_Player = nullptr;

	UPROPERTY()
	TWeakObjectPtr<AWaypointRoute> m_WaypointRoute = nullptr;

	UPROPERTY()
	TMap<EState, TObjectPtr<UAbstractState>> m_States;

	EState m_CurrentState = EState::MAX;

	FVector m_LocationToInvestigate = FVector::ZeroVector;

	UPROPERTY()
	TObjectPtr<UEnemyClassData> m_ClassData = nullptr;

	FBehaviourSettings m_BehaviourSettings;
	FPerceptionSettings m_PerceptionSettings;

	bool m_HasLineOfSight = false;

	UPROPERTY()
	TArray<FEnemyLoadedAbilityData> m_AbilityData;
};
