// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TheAscendance/AI/Enums/State.h"
#include "HSMAgentComponent.generated.h"

class ABaseEnemy;
class APlayerCharacter;
class UAbstractState;
class AWaypointRoute;
class USightSensorComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEASCENDANCE_API UHSMAgentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHSMAgentComponent();

	void Init(ABaseEnemy* owner);
	void InitStats(float visionStrength, float hearingStrength, float preferredDistanceFromTarget, float preferredDistanceTolerance);

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

	void SetVisionStrength(float visionStrength);
	float GetVisionStrength() const;

	void SetHearingStrength(float hearingStrength);
	float GetHearingStrength() const;

	float GetRandomCombatReactionTime() const;

	bool HasLineOfSight() const;
	void SetHasLineOfSight(bool hasLineOfSight);

	bool IsSoundHeard(float soundWeight) const;
	bool IsInCombat() const;


	// Called every frame
	virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<USightSensorComponent> m_SightSensor = nullptr;

	TWeakObjectPtr<ABaseEnemy> m_Owner = nullptr;
	TWeakObjectPtr<APlayerCharacter> m_Player = nullptr;

	//Temp until better setup
	TWeakObjectPtr<AWaypointRoute> m_WaypointRoute = nullptr;

	UPROPERTY()
	TMap<EState, TObjectPtr<UAbstractState>> m_States;

	EState m_CurrentState = EState::MAX;

	FVector m_LocationToInvestigate = FVector::ZeroVector;

	float m_VisionStrength = 0.0f;
	float m_HearingStrength = 0.0f;

	float m_PreferredDistanceFromTarget = 0.0f;
	float m_PreferredDistanceTolerance = 0.0f;

	float m_CombatReactionTimeMin = 0.0f;
	float m_CombatReactionTimeMax = 0.0f;

	bool m_HasLineOfSight = false;
};
