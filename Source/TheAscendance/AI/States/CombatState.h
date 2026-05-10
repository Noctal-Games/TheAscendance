// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbstractState.h"
#include "TheAscendance/AI/Enums/State.h"
#include "TheAscendance/AI/Actions/Attacks/Structs/AttackData.h"
#include "CombatState.generated.h"

class UHSMAgentComponent;

UCLASS()
class THEASCENDANCE_API UCombatState : public UAbstractState
{
	GENERATED_BODY()

public:
	void Init(const FLoadedCombatSettings& combatSettings);

	virtual void StartState(UHSMAgentComponent* owningAgent) override;
	virtual void Update(float deltaTime) override;
	virtual void EndState() override;

	void SetCombatState(ECombatState newState);

	virtual const FString GetStateToString() const override;

private:
	void SetCurrentAbilityData(const FLoadedAbilityData* abilityData);

private:
	UPROPERTY()
	TMap<ECombatState, TObjectPtr<UAbstractState>> m_CombatStates;
	ECombatState m_CurrentCombatState = ECombatState::MAX;

	FLoadedCombatSettings m_CombatSettings;
};
