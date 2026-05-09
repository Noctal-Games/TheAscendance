// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TheAscendance/Characters/Enemies/Structs/EnemyData.h"
#include "TheAscendance/AI/Perception/Enums/SightBodyParts.h"
#include "PerceptionComponent.generated.h"

class ABaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEASCENDANCE_API UPerceptionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPerceptionComponent();

	void Init(const FPerceptionSettings& perceptionSettings);
	void SetTarget(ABaseCharacter* newTarget);
	void SetIsActive(bool isActive);

	bool IsTargetDetected() const;

	bool IsSoundHeard(const float soundWeight) const;
	bool HasLineOfSight() const;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void UpdatePerception();
	bool CheckForLineOfSight();

private:	
	UPROPERTY()
	TWeakObjectPtr<ABaseCharacter> m_Target = nullptr;

	UPROPERTY()
	TMap<ESightBodyParts, float> m_BodyPartWeights;

	FPerceptionSettings m_PerceptionSettings;

	FTimerHandle m_SightCheckTimerHandle;

	float m_CachedHalfFOV = 0.0f;
	float m_CurrentDetection = 0.0f;

	bool m_HasLineOfSight = false;
	bool m_IsTargetDetected = false;
	bool m_IsActive = false;
};
