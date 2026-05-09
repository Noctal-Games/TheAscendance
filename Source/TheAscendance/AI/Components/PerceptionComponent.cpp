// Fill out your copyright notice in the Description page of Project Settings.


#include "PerceptionComponent.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Characters/BaseCharacter.h"
#include "TheAscendance/AI/AIHelperSubsystem.h"

// Sets default values for this component's properties
UPerceptionComponent::UPerceptionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UPerceptionComponent::Init(const FPerceptionSettings& perceptionSettings)
{
	m_PerceptionSettings = perceptionSettings;

	m_CachedHalfFOV = FMath::Cos(FMath::DegreesToRadians(m_PerceptionSettings.FOV * 0.5f));

	m_BodyPartWeights.Add(ESightBodyParts::HEAD, 0.4f);
	m_BodyPartWeights.Add(ESightBodyParts::CHEST, 0.6f);
	m_BodyPartWeights.Add(ESightBodyParts::L_ARM, 0.2f);
	m_BodyPartWeights.Add(ESightBodyParts::R_ARM, 0.2f);
	m_BodyPartWeights.Add(ESightBodyParts::L_LEG, 0.3f);
	m_BodyPartWeights.Add(ESightBodyParts::R_LEG, 0.3f);
}

void UPerceptionComponent::SetTarget(ABaseCharacter* newTarget)
{
	m_Target = newTarget;
}

void UPerceptionComponent::SetIsActive(bool isActive)
{
	m_IsActive = isActive;

	if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		if (m_IsActive && world->GetTimerManager().IsTimerActive(m_SightCheckTimerHandle) == false)
		{
			world->GetTimerManager().SetTimer(m_SightCheckTimerHandle, this, &UPerceptionComponent::UpdatePerception, 0.5f, true);
		}
		else
		{
			world->GetTimerManager().ClearTimer(m_SightCheckTimerHandle);
		}
	}
}

bool UPerceptionComponent::IsTargetDetected() const
{
	return m_IsTargetDetected;
}

bool UPerceptionComponent::IsSoundHeard(const float soundWeight) const
{
	return m_PerceptionSettings.HearingStrength > (1 - soundWeight);
}

bool UPerceptionComponent::HasLineOfSight() const
{
	return m_HasLineOfSight;
}

// Called when the game starts
void UPerceptionComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UPerceptionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (m_HasLineOfSight == false)
	{
		m_CurrentDetection -= m_PerceptionSettings.DetectionLossRate * DeltaTime;
	}
	else
	{
		m_CurrentDetection += m_PerceptionSettings.DetectionGainRate * DeltaTime;
	}

	FMath::Clamp(m_CurrentDetection, 0.0f, 1.0f);
	m_IsTargetDetected = m_CurrentDetection >= m_PerceptionSettings.DetectionThreshold;
}


void UPerceptionComponent::UpdatePerception()
{
	m_HasLineOfSight = CheckForLineOfSight();
}

bool UPerceptionComponent::CheckForLineOfSight()
{
	if (m_Target == nullptr)
	{
		return false;
	}

	AActor* owner = GetOwner();

	if (owner == nullptr)
	{
		return false;
	}

	UAIHelperSubsystem* aiHelper = nullptr;

	UWorld* world = UCoreFunctionLibrary::GetGameWorld();
	
	if (world == nullptr)
	{
		return false;
	}

	aiHelper = world->GetSubsystem<UAIHelperSubsystem>();

	if (aiHelper == nullptr)
	{
		return false;
	}

	const FVector ownerLocation = owner->GetActorLocation();
	const FVector targetLocation = m_Target->GetActorLocation();

	FVector agentEyeLocation = FVector::ZeroVector;
	FRotator agentEyeRotation = FRotator::ZeroRotator;

	owner->GetActorEyesViewPoint(agentEyeLocation, agentEyeRotation);

	const float distance = FVector::Distance(ownerLocation, targetLocation);

	if (distance > m_PerceptionSettings.SightMaxDistance)
	{
		return false;
	}

	const FVector dir = (targetLocation - ownerLocation).GetSafeNormal();

	FVector forward = owner->GetActorForwardVector();
	const float dot = FVector::DotProduct(forward, dir);

	if (dot < m_CachedHalfFOV)
	{
		return false;
	}

	const float normalizedDistance = distance / m_PerceptionSettings.SightMaxDistance;
	float distanceVisibilityMultiplier = 1.0f;

	if (m_PerceptionSettings.DistanceVisibilityCurve)
	{
		distanceVisibilityMultiplier = m_PerceptionSettings.DistanceVisibilityCurve->GetFloatValue(normalizedDistance);
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

		if (world->LineTraceSingleByChannel(hit, agentEyeLocation, end, ECC_Pawn, params))
		{
			if (hit.GetActor() == nullptr || hit.GetActor() != m_Target)
			{
				UCoreFunctionLibrary::DrawDebugLine(agentEyeLocation, end, FColor::Red, 0.5f);
				continue;
			}

			UCoreFunctionLibrary::DrawDebugLine(agentEyeLocation, end, FColor::Green, 0.5f);

			const float bodyPartWeight = aiHelper->GetBodyPartSightWeight(part);
			visibleWeight += bodyPartWeight * distanceVisibilityMultiplier;

			if (visibleWeight >= m_PerceptionSettings.SightStrength)
			{
				break;
			}
		}
		else
		{
			UCoreFunctionLibrary::DrawDebugLine(agentEyeLocation, end, FColor::Red, 0.5f);
		}
	}

	return visibleWeight >= m_PerceptionSettings.SightStrength;
}

