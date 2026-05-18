// Fill out your copyright notice in the Description page of Project Settings.


#include "StrafeCombatState.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/AI/Combat/Components/CombatAIComponent.h"

void UStrafeCombatState::StartState()
{
	UAbstractCombatState::StartState();
	
	m_CurrentTime = 0.0f;
	m_StrafeDuration = FMath::FRandRange(1.5f, 3.5f); //Add variable in data rather than hard code

	ChooseDirection();

	if (m_OwnerComponent != nullptr)
	{
		m_OwnerComponent->LookAtTarget();
		m_OwnerComponent->RequestWaitForState();
		m_OwnerComponent->SetIsAIMovementPaused(false);
		//Strafe speed
	}
}

void UStrafeCombatState::EndState()
{
	if (m_OwnerComponent != nullptr)
	{
		m_OwnerComponent->NotifyStateFinished();
		//Strafe speed
	}

	//Normal speed
	UAbstractCombatState::EndState();
}

void UStrafeCombatState::Update(float deltaTime)
{
	UAbstractCombatState::Update(deltaTime);

	if (m_OwnerComponent == nullptr)
	{
		LOG_ERROR("[STRAFE COMBAT STATE] OwnerComponent is invalid");
		return;
	}

	m_CurrentTime += deltaTime;

	AActor* target = m_OwnerComponent->GetCurrentTarget();

	if (target == nullptr)
	{
		m_OwnerComponent->SetState(ECombatState::IDLE);
		return;
	}

	if (ShouldFlipDirection() == true)
	{
		m_StrafeDirection *= -1.0f;
	}

	UpdateMovement();

	if (m_CurrentTime >= m_StrafeDuration)
	{
		m_OwnerComponent->NotifyStateFinished();
	}
}

const FString UStrafeCombatState::GetStateToString() const
{
	return FString("STRAFE COMBAT STATE");
}

void UStrafeCombatState::UpdateMovement()
{
	if (m_OwnerComponent == nullptr)
	{
		return;
	}

	AActor* target = m_OwnerComponent->GetCurrentTarget();

	if (target == nullptr)
	{
		return;
	}

	AActor* owner = m_OwnerComponent->GetOwner();

	if (owner == nullptr)
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

	FVector desiredDirection = rightVector * m_StrafeDirection;

	const float distance = FVector::Distance(ownerLocation, targetLocation);
	float preferredRange = 0.0f;// m_CombatComponent->GetPreferredEngagementRange();
	float tolerance = 0.0f;// m_CombatComponent->GetEngagementRangeTolerance();
	float maxRange = 0.0f;

	m_OwnerComponent->GetCombatRangeValues(preferredRange, maxRange, tolerance);

	const float rangeDelta = distance - preferredRange;

	const float distanceRatio = FMath::Abs(rangeDelta) / FMath::Max(preferredRange, 1.0f);
	const float rangeUrgency = FMath::Clamp(distanceRatio, 0.0f, 1.0f);

	const float aggression = m_OwnerComponent->GetAggression();
	const float defensiveWeight = 1.0f - aggression;

	const float rangeSensitivity = 1.0f - tolerance;
	const float rangeCorrectionStrength = rangeUrgency * rangeSensitivity * defensiveWeight;

	// Too far
	if (rangeDelta > 0.0f)
	{
		desiredDirection += toTarget * rangeCorrectionStrength;
	}
	// Too close
	else
	{
		desiredDirection -= toTarget * rangeCorrectionStrength;
	}

	desiredDirection.Normalize();
	m_OwnerComponent->m_MovementIntent = desiredDirection;

	//const FVector destination = ownerLocation + (desiredDirection * 250.0f);
	//m_OwnerComponent->SetDestination(destination);
}

void UStrafeCombatState::ChooseDirection()
{
	m_StrafeDirection = FMath::RandBool() ? 1.0f : -1.0f;
}

bool UStrafeCombatState::ShouldFlipDirection() const
{
	return FMath::FRandRange(0.0f, 1.0f) <= 0.01f;
}
