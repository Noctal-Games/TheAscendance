// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMovementComponent.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "PlayerCharacter.h"

#include "Components/CapsuleComponent.h"

void UPlayerMovementComponent::UpdateCharacterStateBeforeMovement(float deltaSeconds)
{
	TrySprinting();
	TryCrouching();

	Super::UpdateCharacterStateBeforeMovement(deltaSeconds);
}

void UPlayerMovementComponent::UpdateCharacterStateAfterMovement(float deltaSeconds)
{
	Super::UpdateCharacterStateAfterMovement(deltaSeconds);
}

void UPlayerMovementComponent::TrySprinting()
{
	m_IsSprinting = CanSprint();

	if (m_IsSprinting == false)
	{
		return;
	}

	if (m_IsCrouching == true)
	{
		SetMovementMode(MOVE_Custom, CMOVE_CROUCH_SPRINTING);
	}
	else
	{
		SetMovementMode(MOVE_Custom, CMOVE_SPRINTING);
	}
}

bool UPlayerMovementComponent::CanSprint()
{
	if (m_PlayerCharacter.IsValid() == false)
	{
		LOG_ERROR("PlayerMovementComponent has lost it's reference to the PlayerCharacter");
		return false;
	}

	if (IsFalling() == true || m_PlayerCharacter->IsSprinting() == false)
	{
		return false;
	}

	FVector direction = Velocity;
	direction.Normalize();

	if (FVector::DotProduct(m_PlayerCharacter->GetActorForwardVector(), direction) <= 0.1)
	{
		return false;
	}

	// Check if the character is on walkable floor
	FFindFloorResult floorResult;
	FindFloor(UpdatedComponent->GetComponentLocation(), floorResult, false);

	return floorResult.bWalkableFloor;
}

void UPlayerMovementComponent::PhysSprinting(float deltaTime, int32 iterations)
{
	if (m_IsSprinting == false)
	{
		// Restore default walk variables
		SetMovementMode(DefaultLandMovementMode);
	}

	PhysWalking(deltaTime, iterations);
}

void UPlayerMovementComponent::TryCrouching()
{
	m_IsCrouching = CanCrouch();

	if (m_IsCrouching == false)
	{
		return;
	}

	if (m_IsSprinting == true)
	{
		SetMovementMode(MOVE_Custom, CMOVE_CROUCH_SPRINTING);
	}
	else
	{
		SetMovementMode(MOVE_Custom, CMOVE_CROUCHING);
	}
}
bool UPlayerMovementComponent::CanCrouch()
{
	if (m_PlayerCharacter.IsValid() == false)
	{
		LOG_ERROR("PlayerMovementComponent has lost it's reference to the PlayerCharacter");
		return false;
	}

	if (IsFalling() == true || (m_PlayerCharacter->IsCrouching() == false && (m_PlayerCharacter->IsCrouched() == false || m_PlayerCharacter->IsCrouched() == true && CanUnCrouch() == true)))
	{
		return false;
	}

	// Check if the character is on walkable floor
	FFindFloorResult floorResult;
	FindFloor(UpdatedComponent->GetComponentLocation(), floorResult, false);

	return floorResult.bWalkableFloor;
}

void UPlayerMovementComponent::PhysCrouching(float deltaTime, int32 iterations)
{
	if (m_IsCrouching == false)
	{
		// Restore default walk variables
		SetMovementMode(DefaultLandMovementMode);
	}

	PhysWalking(deltaTime, iterations);
}

bool UPlayerMovementComponent::CanUnCrouch()
{
	if (m_PlayerCharacter.IsValid() == false)
	{
		LOG_ERROR("PlayerMovementComponent has lost it's reference to the PlayerCharacter");
		return false;
	}

	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner());

	FVector transitionTarget = GetActorLocation();
	transitionTarget.Z += m_PlayerCharacter->GetDefaultCapsuleHeight();

	FCollisionShape capShape = FCollisionShape::MakeCapsule(GetCapsuleRadius(), GetCapsuleHalfHeight());

	//DrawDebugCapsule(GetWorld(), transitionTarget, GetCapsuleHalfHeight(), GetCapsuleRadius(), FQuat::Identity, FColor::Green, false, 3);

	if (GetWorld()->OverlapAnyTestByProfile(transitionTarget, FQuat::Identity, "BlockAll", capShape, params) == true)
	{
		return false;
	}

	return true;
}

float UPlayerMovementComponent::GetSpeed() 
{
	return Velocity.Length();
}

bool UPlayerMovementComponent::IsWalkingCustom() 
{
	if (!IsWalking() || FMath::IsNearlyZero(GetSpeed()))
	{
		return false;
	}

	if (IsSprinting())
	{
		return false;
	}

	return true;
}

bool UPlayerMovementComponent::IsSprinting() 
{
	return m_IsSprinting;
}
bool UPlayerMovementComponent::IsCrouchingCustom() 
{
	return m_IsCrouching;
}

bool UPlayerMovementComponent::IsCustomMovementMode() 
{
	return MovementMode == MOVE_Custom;
}
float UPlayerMovementComponent::GetCapsuleRadius() 
{
	return CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius();
}
float UPlayerMovementComponent::GetCapsuleHalfHeight() 
{
	return CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}
float UPlayerMovementComponent::GetMaxSpeed() const
{
	const float maxSpeed = MaxWalkSpeed;

	if (MovementMode != MOVE_Custom)
	{
		return maxSpeed;
	}

	switch (CustomMovementMode)
	{
	case CMOVE_SPRINTING:
		return  maxSpeed + m_PlayerCharacter->GetStat(ECharacterStat::SPRINT_SPEED_BONUS);
	case CMOVE_CROUCHING:
		return maxSpeed - m_PlayerCharacter->GetStat(ECharacterStat::CROUCH_SPEED_PENALTY);
	case CMOVE_CROUCH_SPRINTING:
		return (maxSpeed - m_PlayerCharacter->GetStat(ECharacterStat::CROUCH_SPEED_PENALTY)) + m_PlayerCharacter->GetStat(ECharacterStat::SPRINT_SPEED_BONUS);
	default:
		return maxSpeed;
	}
}
void UPlayerMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	m_PlayerCharacter = Cast<APlayerCharacter>(GetOwner());

	if (m_PlayerCharacter.IsValid() == false)
	{
		LOG_ERROR("PlayerController failed to store reference to PlayerCharacter");
	}

	m_LastMode = EMovementMode::MOVE_Walking;
}

void UPlayerMovementComponent::PhysCustom(float deltaTime, int32 iterations)
{
	Super::PhysCustom(deltaTime, iterations);

	switch (CustomMovementMode)
	{
	case CMOVE_SPRINTING:
		PhysSprinting(deltaTime, iterations);
		break;
	case CMOVE_CROUCHING:
		PhysCrouching(deltaTime, iterations);
		break;
	case CMOVE_CROUCH_SPRINTING:
		PhysCrouching(deltaTime, iterations);
		break;
	}
}