// Fill out your copyright notice in the Description page of Project Settings.


#include "HeaderWaypoint.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Characters/BaseCharacter.h"
#include "Components/ArrowComponent.h"

AHeaderWaypoint::AHeaderWaypoint() : AWaypoint()
{
	m_WaypointHeader = CreateDefaultSubobject<UArrowComponent>(TEXT("Waypoint Header"));
	checkf(m_WaypointHeader, TEXT("HeaderWaypoint Header failed to initialise"));
	m_WaypointHeader->SetHiddenInGame(false);
	m_WaypointHeader->SetupAttachment(RootComponent);
}

void AHeaderWaypoint::OnCharacterArrived(ABaseCharacter* character)
{
	if(character == nullptr)
	{
		LOG_ERROR("Tried to process Waypoint arrival for invalid BaseCharacter");
		return;
	}

	if (m_HasPathingDelay == true)
	{
		character->TurnTowards(m_WaypointHeader->GetComponentRotation());
	}

	AWaypoint::OnCharacterArrived(character);
}
