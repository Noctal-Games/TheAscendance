// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaypointRoute.generated.h"

class ABaseCharacter;
class AWaypoint;

UCLASS()
class THEASCENDANCE_API AWaypointRoute : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaypointRoute();

	void AddCharacterToRoute(ABaseCharacter* character, int startWaypointIndex = -1);
	void RemoveCharacterFromRoute(ABaseCharacter* character);

	void UpdateCharacterRoute(ABaseCharacter* character);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void SetCharacterWaypoint(ABaseCharacter* character, int waypointIndex);
	int GetIndexOfClosestWaypointToCharacter(ABaseCharacter* character) const;

private:
	UPROPERTY(EditInstanceOnly)
	TArray<TObjectPtr<AWaypoint>> m_Waypoints;

	TMap<TWeakObjectPtr<ABaseCharacter>, int> m_CharactersUsingRoute;
};
