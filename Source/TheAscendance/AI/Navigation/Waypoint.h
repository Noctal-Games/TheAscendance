// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Waypoint.generated.h"

class ABaseCharacter;
class AWaypointRoute;
class UBoxComponent;

UCLASS()
class THEASCENDANCE_API AWaypoint : public AActor
{
	GENERATED_BODY()
	
public:	
 	AWaypoint();

	void Init(AWaypointRoute* parentRoute);

	void AddExpectedCharacter(ABaseCharacter* character);
	void RemoveExpectedCharacter(ABaseCharacter* character);

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnCharacterArrived(ABaseCharacter* character);

protected:
	UPROPERTY(EditInstanceOnly, meta = (DisplayName = "HasPathingDelay"))
	bool m_HasPathingDelay = false;

	TWeakObjectPtr<AWaypointRoute> m_ParentRoute = nullptr;

	TArray<TWeakObjectPtr<ABaseCharacter>> m_ExpectedCharacters;

	UPROPERTY(EditInstanceOnly)
	TObjectPtr<UBoxComponent> m_WaypointCollider = nullptr;

	UPROPERTY(EditInstanceOnly, meta = (DisplayName = "PathingDelay", ClampMin = "2.5", EditCondition = "m_HasPathingDelay == true", EditConditionHides))
	float m_PathingDelay = 3.0f;
};
