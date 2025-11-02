// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TheAscendance/Characters/AI/Enums/State.h"
#include "HSMAgentComponent.generated.h"

class ABaseEnemy;
class APlayerCharacter;
class UAbstractState;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEASCENDANCE_API UHSMAgentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHSMAgentComponent();

	void Init(ABaseEnemy* owner);
	void SetState(EState newState);

	void SetDestination(const FVector& destination);
	const ABaseEnemy* GetAgentOwner() const;

	bool HasPath() const;

	// Called every frame
	virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<ABaseEnemy> m_Owner = nullptr;
	TWeakObjectPtr<APlayerCharacter> m_Player = nullptr;

	UPROPERTY()
	TMap<EState, TObjectPtr<UAbstractState>> m_States;

	EState m_CurrentState = EState::MAX;
};
