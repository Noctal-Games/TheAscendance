// Fill out your copyright notice in the Description page of Project Settings.


#include "Waypoint.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Characters/BaseCharacter.h"
#include "WaypointRoute.h"
#include "Components/BoxComponent.h"

// Sets default values
AWaypoint::AWaypoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_WaypointCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Waypoint Collider"));
	checkf(m_WaypointCollider, TEXT("Waypoint Collider failed to initialise"));
	m_WaypointCollider->SetCollisionProfileName(TEXT("OverlapAll")); //Change to custom for characters only
	SetRootComponent(m_WaypointCollider);
}

void AWaypoint::Init(AWaypointRoute* parentRoute)
{
	if(parentRoute == nullptr)
	{
		LOG_ERROR("Tried to init Waypoint with invalid WaypointRoute");
		return;
	}

	m_ParentRoute = parentRoute;
}

void AWaypoint::AddExpectedCharacter(ABaseCharacter* character)
{
	if(m_ExpectedCharacters.Contains(character) == true)
	{
		return;
	}

	m_ExpectedCharacters.Add(character);
}

void AWaypoint::RemoveExpectedCharacter(ABaseCharacter* character)
{
	if (m_ExpectedCharacters.Contains(character) == false)
	{
		return;
	}

	m_ExpectedCharacters.Remove(character);
}

void AWaypoint::BeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (otherActor == nullptr || otherActor == this || m_ExpectedCharacters.Contains(otherActor) == false)
	{
		return;
	}

	if(ABaseCharacter* character = Cast<ABaseCharacter>(otherActor))
	{
		OnCharacterArrived(character);
	}
	else
	{
		LOG_WARNING("Waypoint overlapped by non-BaseCharacter actor");
	}
}

// Called when the game starts or when spawned
void AWaypoint::BeginPlay()
{
	Super::BeginPlay();
	
	m_WaypointCollider->OnComponentBeginOverlap.AddDynamic(this, &AWaypoint::BeginOverlap);
}

void AWaypoint::OnCharacterArrived(ABaseCharacter* character)
{
	if(character == nullptr)
	{
		LOG_ERROR("Tried to process Waypoint arrival for invalid BaseCharacter");
		return;
	}

	character->SetDestination(character->GetActorLocation());
	m_ExpectedCharacters.Remove(character);

	if(m_HasPathingDelay == false)
	{
		if (m_ParentRoute.IsValid() == false)
		{
			return;
		}

		m_ParentRoute->UpdateCharacterRoute(character);
		return;
	}

	FTimerHandle delayHandle;
	FTimerDelegate delayDelegate;

	delayDelegate.BindLambda([this, character]()
		{
			if (character == nullptr || m_ParentRoute.IsValid() == false)
			{
				return;
			}

			m_ParentRoute->UpdateCharacterRoute(character);
		});

	if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		world->GetTimerManager().SetTimer(delayHandle, delayDelegate, m_PathingDelay, false);
	}
}

