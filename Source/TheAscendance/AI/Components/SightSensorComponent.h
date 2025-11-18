// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TheAscendance/Ai/Perception/Enums/SightBodyParts.h"
#include "SightSensorComponent.generated.h"

class UHSMAgentComponent;
class ABaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEASCENDANCE_API USightSensorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USightSensorComponent();

	void Init(UHSMAgentComponent* ownerAgent);
	void SetTarget(ABaseCharacter* newTarget);
	void SetIsActive(bool isActive);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void SetAgentLineOfSight();
	bool CheckForLineOfSight();

private:
	const float m_DistanceScalar = 10000.0f;

	TWeakObjectPtr<UHSMAgentComponent> m_Agent = nullptr;
	TWeakObjectPtr<ABaseCharacter> m_Target = nullptr;

	TMap<ESightBodyParts, float> m_BodyPartWeights;

	float m_Fov = 0.5f;

	bool m_IsActive = false;

	FTimerHandle m_SightCheckTimerHandle;
};
