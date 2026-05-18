// Fill out your copyright notice in the Description page of Project Settings.


#include "RetreatCombatState.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/AI/Combat/Components/CombatAIComponent.h"

void URetreatCombatState::StartState()
{
	UAbstractCombatState::StartState();

	m_CurrentTime = 0.0f;

	if (m_OwnerComponent != nullptr)
	{
		m_RepathTimer = m_OwnerComponent->BehaviourSettings.ReactionTime.GetRandomValue();
	}

	m_StateDuration = FMath::FRandRange(0.8f, 2.2f);
}

void URetreatCombatState::EndState()
{
	UAbstractCombatState::EndState();
}

void URetreatCombatState::Update(float deltaTime)
{
	UAbstractCombatState::Update(deltaTime);

	if (m_OwnerComponent == nullptr)
	{
		LOG_ERROR("[STRAFE COMBAT STATE] OwnerComponent is invalid");
		return;
	}

	m_CurrentTime += deltaTime;
	m_RepathTimer -= deltaTime;

	AActor* target = m_OwnerComponent->GetCurrentTarget();

	if (target == nullptr)
	{
		m_OwnerComponent->NotifyStateFinished();
		return;
	}

	if (m_CurrentTime >= m_StateDuration)
	{
		m_OwnerComponent->NotifyStateFinished();
	}

	if (m_RepathTimer <= 0.0f)
	{
		m_RepathTimer = m_RepathInterval;
		m_RepathTimer = m_OwnerComponent->BehaviourSettings.ReactionTime.GetRandomValue();
		UpdateMovement();
	}
}

const FString URetreatCombatState::GetStateToString() const
{
	return FString("RETREAT COMBAT STATE");
}

void URetreatCombatState::UpdateMovement()
{
	if (m_OwnerComponent == nullptr)
	{
		return;
	}

	UWorld* world = UCoreFunctionLibrary::GetGameWorld();

	if (world == nullptr)
	{
		return;
	}

	AActor* owner = m_OwnerComponent->GetOwner();

	if (owner == nullptr)
	{
		return;
	}

	AActor* target = m_OwnerComponent->GetCurrentTarget();

	if (target == nullptr)
	{
		return;
	}

	const FVector ownerLocation = owner->GetActorLocation();
	const FVector targetLocation = target->GetActorLocation();

	FVector toTarget = (targetLocation - ownerLocation);
	toTarget.Z = 0.0f;
	toTarget.Normalize();

	// Perpendicular vector for circling
	FVector rightVector = FVector::CrossProduct(FVector::UpVector, toTarget);
	rightVector.Normalize();

	const FVector retreatDirection = -toTarget;

	const float lateralNoise = FMath::Sin(world->TimeSeconds * 2.0f) * 0.35f;

	FVector desiredDirection = retreatDirection + (rightVector * lateralNoise);
	desiredDirection.Normalize();

	m_OwnerComponent->m_MovementIntent = desiredDirection;

	//const float deltaTime = world->GetDeltaSeconds();

	//const float smoothingSpeed = 6.0f;

	//const float alpha = 1.0f - FMath::Exp(-smoothingSpeed * deltaTime);

	//m_SmoothedDirection = FMath::Lerp(m_SmoothedDirection, desiredDirection, alpha);
	//m_SmoothedDirection.Normalize();

	//const FVector destination = ownerLocation + (m_SmoothedDirection * 250.0f);

	//m_OwnerComponent->SetDestination(destination);
}

FVector URetreatCombatState::CalculateRetreatDirection(const FVector& toTarget, float distance, float preferredRange, float aggression) const
{
	const float rangeDelta = distance - preferredRange;
	const float distanceRatio = FMath::Clamp(FMath::Abs(rangeDelta) / FMath::Max(preferredRange, 1.0f), 0.0f, 1.0f);

	//Low aggression values creating as much space as possible
	const float defensiveWeight = 1.0f - aggression;
	const float retreatCommitment = distanceRatio * defensiveWeight;
	const float lateralStrength = aggression * (1.0f - distanceRatio);

	FVector retreatDirection = -toTarget;

	FVector rightVector = FVector::CrossProduct(FVector::UpVector, toTarget);
	rightVector.Normalize();

	FVector finalDirection;

	if (aggression < 0.3f)
	{
		finalDirection = retreatDirection;
	}
	else if (aggression < 0.7f)
	{
		finalDirection = FMath::Lerp(retreatDirection, rightVector, lateralStrength);
	}
	else
	{
		finalDirection = rightVector * (FMath::RandBool() ? 1.0f : -1.0f);
	}

	finalDirection += retreatDirection * retreatCommitment;
	finalDirection.Normalize();

	//const float lateralInfluence = FMath::Clamp(lateralStrength, 0.0f, 1.0f);

	//const float forwardInfluence = 1.0f - lateralInfluence;

	//FVector finalDirection = (retreatDirection * forwardInfluence) + (rightVector * lateralInfluence * (FMath::RandBool() ? 1.0f : -1.0f));

	//finalDirection += retreatDirection * retreatCommitment * distanceRatio;
	//finalDirection.Normalize();

	return finalDirection;
}
