// Fill out your copyright notice in the Description page of Project Settings.


#include "WaypointRoute.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "Waypoint.h"
#include "TheAscendance/Characters/BaseCharacter.h"

// Sets default values
AWaypointRoute::AWaypointRoute()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AWaypointRoute::AddCharacterToRoute(ABaseCharacter* character, int startWaypointIndex)
{
	if(character == nullptr || m_CharactersUsingRoute.Contains(character) == true)
	{
		return;
	}

	m_CharactersUsingRoute.Add(character);

	int startingWaypoint = (startWaypointIndex < 0) ? GetIndexOfClosestWaypointToCharacter(character) : startWaypointIndex;
	SetCharacterWaypoint(character, startingWaypoint);
}

void AWaypointRoute::RemoveCharacterFromRoute(ABaseCharacter* character)
{
	if (character == nullptr || m_CharactersUsingRoute.Contains(character) == false)
	{
		return;
	}

	if (m_Waypoints.IsEmpty() == false && m_CharactersUsingRoute[character] < m_Waypoints.Num() && m_Waypoints[m_CharactersUsingRoute[character]] != nullptr)
	{
		m_Waypoints[m_CharactersUsingRoute[character]]->RemoveExpectedCharacter(character);
	}

	m_CharactersUsingRoute.Remove(character);
	character->SetDestination(character->GetActorLocation());
}

void AWaypointRoute::UpdateCharacterRoute(ABaseCharacter* character)
{
	if (character == nullptr || m_CharactersUsingRoute.Contains(character) == false)
	{
		return;
	}

	if (m_Waypoints.IsEmpty() == false && m_CharactersUsingRoute[character] < m_Waypoints.Num() && m_Waypoints[m_CharactersUsingRoute[character]] != nullptr)
	{
		m_Waypoints[m_CharactersUsingRoute[character]]->RemoveExpectedCharacter(character);
	}

	SetCharacterWaypoint(character, m_CharactersUsingRoute[character] + 1);
}

void AWaypointRoute::SetCharacterWaypoint(ABaseCharacter* character, int waypointIndex)
{
	if (character == nullptr || m_CharactersUsingRoute.Contains(character) == false)
	{
		return;
	}

	int nextWaypointIndex = waypointIndex >= m_Waypoints.Num() ? 0 : waypointIndex;
	m_CharactersUsingRoute[character] = nextWaypointIndex;

	if (m_Waypoints.IsEmpty() == false && m_CharactersUsingRoute[character] < m_Waypoints.Num() && m_Waypoints[m_CharactersUsingRoute[character]] != nullptr)
	{
		m_Waypoints[m_CharactersUsingRoute[character]]->AddExpectedCharacter(character);
		character->SetDestination(m_Waypoints[m_CharactersUsingRoute[character]]->GetActorLocation());
	}
}

int AWaypointRoute::GetIndexOfClosestWaypointToCharacter(ABaseCharacter* character) const
{
	if(character == nullptr || m_Waypoints.IsEmpty() == true)
	{
		return 0;
	}

	float closestDistance = TNumericLimits<float>::Max();
	int closestIndex = 0;

	for (int i = 0; i < m_Waypoints.Num(); i++)
	{
		AWaypoint* waypoint = m_Waypoints[i];

		if (waypoint == nullptr)
		{
			LOG_ERROR("WaypointRoute has invalid Waypoint at index %i", i);
			continue;
		}

		const float distance = FVector::Dist(character->GetActorLocation(), waypoint->GetActorLocation());

		if (distance < closestDistance)
		{
			closestDistance = distance;
			closestIndex = i;
		}
	}

	return closestIndex;
}

// Called when the game starts or when spawned
void AWaypointRoute::BeginPlay()
{
	Super::BeginPlay();

	if(m_Waypoints.IsEmpty() == true)
	{
		LOG_ERROR("WaypointRoute: %s - Has no waypoints", *GetName());
		return;
	}

	for (AWaypoint* waypoint : m_Waypoints)
	{
		if (waypoint != nullptr)
		{
			waypoint->Init(this);
		}
	}
}

