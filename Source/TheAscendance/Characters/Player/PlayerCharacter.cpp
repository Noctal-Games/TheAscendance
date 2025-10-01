// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "PlayerMovementComponent.h"
#include "TheAscendance/Characters/Components/CharacterStatsComponent.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Items/HeldItem.h"
#include "TheAscendance/Spells/Interfaces/Spell.h"

#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter() : ABaseCharacter()
{
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;

	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	checkf(m_Camera, TEXT("Player Camera failed to initialise"));
	m_Camera->SetupAttachment(GetRootComponent());
	m_Camera->bUsePawnControlRotation = true;
}

void APlayerCharacter::SetPlayerController(ATAPlayerController* PlayerController)
{
	m_PlayerController = PlayerController;
}

ATAPlayerController* APlayerCharacter::GetPlayerController()
{
	return m_PlayerController;
}

void APlayerCharacter::SetIsSprinting(bool val)
{
	m_IsSprinting = val;
}
bool APlayerCharacter::IsSprinting()
{
	if (m_CharacterStatsComponent->GetStatAsValue(ECharacterStat::STAMINA) <= 0)
	{
		return false;
	}

	return m_IsSprinting;
}
void APlayerCharacter::SetIsCrouching(bool val)
{
	m_IsCrouching = val;
}
bool APlayerCharacter::IsCrouching()
{
	if (m_IsJumping == true)
	{
		return false;
	}

	return m_IsCrouching;
}
bool APlayerCharacter::IsCrouched()
{
	return m_CurrentCapsuleHeight < m_DefaultCapsuleHeight - 5;
}
void APlayerCharacter::SetIsJumping()
{
	m_IsJumping = true;
}
bool APlayerCharacter::CanJumpInternal_Implementation() const
{
	if (bIsCrouched == true || m_CharacterStatsComponent->GetStatAsValue(ECharacterStat::STAMINA) < 5)
	{
		return false;
	}

	if (JumpIsAllowedInternal() == false)
	{
		if (m_MovementComponent->IsSprinting() == true)
		{
			return true;
		}

		return false;
	}

	return true;
}
void APlayerCharacter::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();

	m_CharacterStatsComponent->AdjustStatByValue(ECharacterStat::STAMINA, -5);
}
float APlayerCharacter::GetDefaultCapsuleHeight()
{
	return m_DefaultCapsuleHeight;
}
void APlayerCharacter::UpdateCrouchCamera(float deltaTime)
{
	float crouchSpeed = deltaTime * 5;

	if (m_MovementComponent->IsCrouchingCustom() && m_CurrentCapsuleHeight > m_CrouchCapsuleHeight)
	{
		m_CurrentCapsuleHeight = FMath::Lerp(m_CurrentCapsuleHeight, m_CrouchCapsuleHeight, crouchSpeed);
		GetCapsuleComponent()->SetCapsuleSize(m_DefaultCapsuleRadius, m_CurrentCapsuleHeight, true);
	}
	if (m_MovementComponent->IsCrouchingCustom() == false && m_CurrentCapsuleHeight < m_DefaultCapsuleHeight)
	{
		m_CurrentCapsuleHeight = FMath::Lerp(m_CurrentCapsuleHeight, m_DefaultCapsuleHeight, crouchSpeed);
		GetCapsuleComponent()->SetCapsuleSize(m_DefaultCapsuleRadius, m_CurrentCapsuleHeight, true);
	}
}
void APlayerCharacter::OnMovementModeChanged(EMovementMode prevMovementMode, uint8 previousCustomMode)
{
	Super::OnMovementModeChanged(prevMovementMode, previousCustomMode);

	if (prevMovementMode != EMovementMode::MOVE_Falling)
	{
		return;
	}

	m_IsJumping = false;
}

UCameraComponent* APlayerCharacter::GetCamera()
{
	return m_Camera;
}

const FVector APlayerCharacter::GetCastStartForward()
{
	return m_Camera->GetForwardVector();
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	ABaseCharacter::BeginPlay();

	m_MovementComponent = Cast<UPlayerMovementComponent>(GetMovementComponent());
	checkf(m_MovementComponent, TEXT("Movement Component is an invalid value"));

	m_DefaultCapsuleRadius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	m_DefaultCapsuleHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	m_CurrentCapsuleHeight = m_DefaultCapsuleHeight;
	m_CrouchCapsuleHeight = m_DefaultCapsuleHeight / 2;

	//Test
	if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
	{
		if (ISpell* spell = gameMode->CreateSpellFromID(1, this))
		{
			m_TestSpell = spell->_getUObject();
		}
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	ABaseCharacter::Tick(DeltaTime);

	LOG_ONSCREEN(0, 1, FColor::Yellow, "USING ANIMATIONS: %s", m_AnimTest ? TEXT("TRUE") : TEXT("FALSE"));
}

void APlayerCharacter::TestFunction1()
{
	LOG_ONSCREEN(-1, 1.0f, FColor::Yellow, "TEST 1");
	m_AnimTest = !m_AnimTest;
}

void APlayerCharacter::TestFunction2()
{
	LOG_ONSCREEN(-1, 1.0f, FColor::Yellow, "TEST 2");
	//EndMainHandAttack();
	//EndOffHandAttack();

	if (m_TestSpell == nullptr)
	{
		LOG_ONSCREEN(-1, 1.0f, FColor::Red, "TestSpell is invalid");
		return;
	}

	if (m_TestSpell->CanCast() == true)
	{
		m_TestSpell->CastSpell();
	}
}

void APlayerCharacter::TestFunction3()
{
	LOG_ONSCREEN(-1, 1.0f, FColor::Yellow, "TEST 3");

	if (m_TestEquipToggle == false)
	{
		if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
		{
			m_MainHandItem->Init(gameMode->GetItemData(2));
		}
	}
	else
	{
		m_MainHandItem->UnEquip();
	}

	m_TestEquipToggle = !m_TestEquipToggle;
}

