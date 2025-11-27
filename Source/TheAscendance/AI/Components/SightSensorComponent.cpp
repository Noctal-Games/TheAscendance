// Fill out your copyright notice in the Description page of Project Settings.


#include "SightSensorComponent.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Characters/BaseCharacter.h"
#include "TheAscendance/AI/Components/HSMAgentComponent.h"
#include "TheAscendance/AI/AIHelperSubsystem.h"

// Sets default values for this component's properties
USightSensorComponent::USightSensorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void USightSensorComponent::Init(UHSMAgentComponent* ownerAgent)
{
	m_Agent = ownerAgent;
	m_Fov = 0.5;

	m_BodyPartWeights.Add(ESightBodyParts::HEAD, 0.4f);
	m_BodyPartWeights.Add(ESightBodyParts::CHEST, 0.6f);
	m_BodyPartWeights.Add(ESightBodyParts::L_ARM, 0.2f);
	m_BodyPartWeights.Add(ESightBodyParts::R_ARM, 0.2f);
	m_BodyPartWeights.Add(ESightBodyParts::L_LEG, 0.3f);
	m_BodyPartWeights.Add(ESightBodyParts::R_LEG, 0.3f);
}

void USightSensorComponent::SetTarget(ABaseCharacter* newTarget)
{
	m_Target = newTarget;
}

void USightSensorComponent::SetIsActive(bool isActive)
{
	m_IsActive = isActive;

	if(UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		if(m_IsActive && world->GetTimerManager().IsTimerActive(m_SightCheckTimerHandle) == false)
		{
			world->GetTimerManager().SetTimer(m_SightCheckTimerHandle, this, &USightSensorComponent::SetAgentLineOfSight, 0.5f, true);
		}
		else
		{
			world->GetTimerManager().ClearTimer(m_SightCheckTimerHandle);
		}
	}
}

void USightSensorComponent::SetAgentLineOfSight()
{
	m_Agent->SetHasLineOfSight(CheckForLineOfSight());
}

bool USightSensorComponent::CheckForLineOfSight()
{	
	if(m_Target == nullptr || m_Agent == nullptr)
	{
		return false;
	}

	AActor* owner = m_Agent->GetOwner();

	if(owner == nullptr)
	{
		return false;
	}

	UAIHelperSubsystem* aiHelper = nullptr;

	if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		aiHelper = world->GetSubsystem<UAIHelperSubsystem>();
	}

	if(aiHelper == nullptr)
	{
		return false;
	}

	const FVector ownerLocation = owner->GetActorLocation();
	const FVector targetLocation = m_Target->GetActorLocation();

	FVector agentEyeLocation = FVector::ZeroVector;
	FRotator agentEyeRotation = FRotator::ZeroRotator;

	owner->GetActorEyesViewPoint(agentEyeLocation, agentEyeRotation);

	FVector dir = targetLocation - ownerLocation;

	float distance = dir.Length();

	//TODO: Check if the distance between the actor and target is greater than the seeable distance

	dir.Normalize();

	FVector forward = owner->GetActorForwardVector();

	if (forward.Dot(dir) < (1 - m_Fov))
	{
		return false;
	}

	float visibleWeight = 0.0f;

	FHitResult hit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(owner);

	for (int i = 0; i < (int32)ESightBodyParts::MAX; i++)
	{
		ESightBodyParts part = (ESightBodyParts)i;

		FName socketName = aiHelper->GetBodyPartSocketName(part);
		FVector end = aiHelper->GetTargetSocketLocation(m_Target.Get(), socketName);

		if (end == FVector::ZeroVector)
		{
			continue;
		}

		UCoreFunctionLibrary::DrawDebugLine(agentEyeLocation, end, FColor::Green, 0.5f);

		if (GetWorld()->LineTraceSingleByChannel(hit, agentEyeLocation, end, ECC_Pawn, params))
		{
			if (hit.GetActor() == nullptr || hit.GetActor() != m_Target)
			{
				continue;
			}

			visibleWeight += aiHelper->GetBodyPartSightWeight(part) * (1 - (distance / m_DistanceScalar));

			if (visibleWeight > (1 - m_Agent->GetVisionStrength()))
			{
				return true;
			}
		}
	}

	return visibleWeight > (1 - m_Agent->GetVisionStrength());
}

// Called when the game starts
void USightSensorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}



