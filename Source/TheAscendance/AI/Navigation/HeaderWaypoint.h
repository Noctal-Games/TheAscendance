// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Waypoint.h"
#include "HeaderWaypoint.generated.h"

class UArrowComponent;

UCLASS()
class THEASCENDANCE_API AHeaderWaypoint : public AWaypoint
{
	GENERATED_BODY()
	
public:
	AHeaderWaypoint();

protected:
	virtual void OnCharacterArrived(ABaseCharacter* character) override;

private:
	UPROPERTY(VisibleInstanceOnly)
	TObjectPtr<UArrowComponent> m_WaypointHeader = nullptr;
};
