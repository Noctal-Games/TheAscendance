// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomLocalPlayerSubsystem.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Characters/Player/PlayerCharacter.h"
#include "TheAscendance/Characters/Player/CustomPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"

void UCustomLocalPlayerSubsystem::ToggleHUDVisibility(bool isVisible)
{
	// Handle HUD Visibility Toggle
}

void UCustomLocalPlayerSubsystem::SetPlayer(APlayerCharacter* player)
{
	m_PlayerCharacter = player;
}

void UCustomLocalPlayerSubsystem::SetController(ACustomPlayerController* controller)
{
	m_Controller = controller;
}

void UCustomLocalPlayerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UCustomLocalPlayerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UCustomLocalPlayerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (this->GetClass()->IsInBlueprint() && Super::ShouldCreateSubsystem(Outer))
	{
		return true;
	}

	return false;
}

void UCustomLocalPlayerSubsystem::CreateHUD()
{
	// Handle HUD Creation
}

void UCustomLocalPlayerSubsystem::SetupInput(UEnhancedInputComponent* enhancedInputComponent)
{
	BindActions(enhancedInputComponent);

	if (UEnhancedInputLocalPlayerSubsystem* inputSubsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		inputSubsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void UCustomLocalPlayerSubsystem::DestroyHUD()
{	
	// Handle HUD Destruction
}

void UCustomLocalPlayerSubsystem::HandleLook(const FInputActionValue& value)
{
	if (m_Controller.IsValid() == false)
	{
		return;
	}

	const FVector rotationVector = value.Get<FVector>();
	m_Controller->UpdateCamera(rotationVector);
}

void UCustomLocalPlayerSubsystem::HandleMove(const FInputActionValue& value)
{
	if (m_PlayerCharacter.IsValid() == false)
	{
		LOG_ERROR("[LOCAL PLAYER SUBSYSTEM] Invalid PlayerCharacter reference");
		return;
	}

	const FVector movementVector = value.Get<FVector>();

	m_PlayerCharacter->AddMovementInput(m_PlayerCharacter->GetActorForwardVector(), movementVector.Y);
	m_PlayerCharacter->AddMovementInput(m_PlayerCharacter->GetCamera()->GetRightVector(), movementVector.X);
}

void UCustomLocalPlayerSubsystem::HandleJump()
{
	if (m_PlayerCharacter.IsValid() == false)
	{
		LOG_ERROR("[LOCAL PLAYER SUBSYSTEM] Invalid PlayerCharacter reference");
		return;
	}

	m_PlayerCharacter->SetIsJumping();
	m_PlayerCharacter->Jump();
}

void UCustomLocalPlayerSubsystem::HandleStartSprint()
{
	if (m_PlayerCharacter.IsValid() == false)
	{
		LOG_ERROR("[LOCAL PLAYER SUBSYSTEM] Invalid PlayerCharacter reference");
		return;
	}

	m_PlayerCharacter->SetIsSprinting(true);
}

void UCustomLocalPlayerSubsystem::HandleEndSprint()
{
	if (m_PlayerCharacter.IsValid() == false)
	{
		LOG_ERROR("[LOCAL PLAYER SUBSYSTEM] Invalid PlayerCharacter reference");
		return;
	}

	m_PlayerCharacter->SetIsSprinting(false);
}

void UCustomLocalPlayerSubsystem::HandleStartCrouch()
{
	if (m_PlayerCharacter.IsValid() == false)
	{
		LOG_ERROR("[LOCAL PLAYER SUBSYSTEM] Invalid PlayerCharacter reference");
		return;
	}

	m_PlayerCharacter->SetIsCrouching(true);
}

void UCustomLocalPlayerSubsystem::HandleEndCrouch()
{
	if (m_PlayerCharacter.IsValid() == false)
	{
		LOG_ERROR("[LOCAL PLAYER SUBSYSTEM] Invalid PlayerCharacter reference");
		return;
	}

	m_PlayerCharacter->SetIsCrouching(false);
}

void UCustomLocalPlayerSubsystem::HandleMainHandPrimaryAttack()
{
	if (m_PlayerCharacter.IsValid() == false)
	{
		LOG_ERROR("[LOCAL PLAYER SUBSYSTEM] Invalid PlayerCharacter reference");
		return;
	}

	m_PlayerCharacter->MainHandPrimaryAttack();
}

void UCustomLocalPlayerSubsystem::HandleMainHandSecondaryAttack()
{
	if (m_PlayerCharacter.IsValid() == false)
	{
		LOG_ERROR("[LOCAL PLAYER SUBSYSTEM] Invalid PlayerCharacter reference");
		return;
	}

	m_PlayerCharacter->MainHandSecondaryAttack();
}

void UCustomLocalPlayerSubsystem::HandleOffhandPrimaryAttack()
{
	if (m_PlayerCharacter.IsValid() == false)
	{
		LOG_ERROR("[LOCAL PLAYER SUBSYSTEM] Invalid PlayerCharacter reference");
		return;
	}

	m_PlayerCharacter->OffHandPrimaryAttack();
}

void UCustomLocalPlayerSubsystem::HandleOffhandSecondaryAttack()
{
	if (m_PlayerCharacter.IsValid() == false)
	{
		LOG_ERROR("[LOCAL PLAYER SUBSYSTEM] Invalid PlayerCharacter reference");
		return;
	}

	m_PlayerCharacter->OffHandSecondaryAttack();
}

void UCustomLocalPlayerSubsystem::HandleTestFunction1()
{
	if (m_PlayerCharacter.IsValid() == false)
	{
		LOG_ERROR("[LOCAL PLAYER SUBSYSTEM] Invalid PlayerCharacter reference");
		return;
	}

	m_PlayerCharacter->TestFunction1();
}

void UCustomLocalPlayerSubsystem::HandleTestFunction2()
{
	if (m_PlayerCharacter.IsValid() == false)
	{
		LOG_ERROR("[LOCAL PLAYER SUBSYSTEM] Invalid PlayerCharacter reference");
		return;
	}

	m_PlayerCharacter->TestFunction2();
}

void UCustomLocalPlayerSubsystem::HandleTestFunction3()
{
	if (m_PlayerCharacter.IsValid() == false)
	{
		LOG_ERROR("[LOCAL PLAYER SUBSYSTEM] Invalid PlayerCharacter reference");
		return;
	}

	m_PlayerCharacter->TestFunction3();
}

void UCustomLocalPlayerSubsystem::BindActions(UEnhancedInputComponent* enhancedInputComponent)
{
	checkf(ActionLook, TEXT("[LOCAL PLAYER SUBSYSTEM] Missing 'Look' Action"));
	enhancedInputComponent->BindAction(ActionLook, ETriggerEvent::Triggered, this, &UCustomLocalPlayerSubsystem::HandleLook);

	checkf(ActionMove, TEXT("[LOCAL PLAYER SUBSYSTEM] Missing 'Move' Action"));
	enhancedInputComponent->BindAction(ActionMove, ETriggerEvent::Triggered, this, &UCustomLocalPlayerSubsystem::HandleMove);

	checkf(ActionJump, TEXT("[LOCAL PLAYER SUBSYSTEM] Missing 'Jump' Action"));
	enhancedInputComponent->BindAction(ActionJump, ETriggerEvent::Triggered, this, &UCustomLocalPlayerSubsystem::HandleJump);

	checkf(ActionSprint, TEXT("[LOCAL PLAYER SUBSYSTEM] Missing 'Sprint' Action"));
	enhancedInputComponent->BindAction(ActionSprint, ETriggerEvent::Started, this, &UCustomLocalPlayerSubsystem::HandleStartSprint);
	enhancedInputComponent->BindAction(ActionSprint, ETriggerEvent::Completed, this, &UCustomLocalPlayerSubsystem::HandleEndSprint);

	checkf(ActionSprint, TEXT("[LOCAL PLAYER SUBSYSTEM] Missing 'Crouch' Action"));
	enhancedInputComponent->BindAction(ActionCrouch, ETriggerEvent::Started, this, &UCustomLocalPlayerSubsystem::HandleStartCrouch);
	enhancedInputComponent->BindAction(ActionCrouch, ETriggerEvent::Completed, this, &UCustomLocalPlayerSubsystem::HandleEndCrouch);

	checkf(ActionMainHandPrimaryAttack, TEXT("[LOCAL PLAYER SUBSYSTEM] Missing 'Main Hand Primary Attack' Action"));
	enhancedInputComponent->BindAction(ActionMainHandPrimaryAttack, ETriggerEvent::Triggered, this, &UCustomLocalPlayerSubsystem::HandleMainHandPrimaryAttack);
	checkf(ActionMainHandSecondaryAttack, TEXT("[LOCAL PLAYER SUBSYSTEM] Missing 'Main Hand Alt Attack' Action"));
	enhancedInputComponent->BindAction(ActionMainHandSecondaryAttack, ETriggerEvent::Triggered, this, &UCustomLocalPlayerSubsystem::HandleMainHandSecondaryAttack);

	checkf(ActionOffHandPrimaryAttack, TEXT("[LOCAL PLAYER SUBSYSTEM] Missing 'Offhand Primary Attack' Action"));
	enhancedInputComponent->BindAction(ActionOffHandPrimaryAttack, ETriggerEvent::Triggered, this, &UCustomLocalPlayerSubsystem::HandleOffhandPrimaryAttack);
	checkf(ActionOffHandSecondaryAttack, TEXT("[LOCAL PLAYER SUBSYSTEM] Missing 'Offhand Alt Attack' Action"));
	enhancedInputComponent->BindAction(ActionOffHandSecondaryAttack, ETriggerEvent::Triggered, this, &UCustomLocalPlayerSubsystem::HandleOffhandSecondaryAttack);

	checkf(ActionTestFunction1, TEXT("[LOCAL PLAYER SUBSYSTEM] Missing 'TestFunction1' Action"));
	enhancedInputComponent->BindAction(ActionTestFunction1, ETriggerEvent::Triggered, this, &UCustomLocalPlayerSubsystem::HandleTestFunction1);

	checkf(ActionTestFunction2, TEXT("[LOCAL PLAYER SUBSYSTEM] Missing 'TestFunction2' Action"));
	enhancedInputComponent->BindAction(ActionTestFunction2, ETriggerEvent::Triggered, this, &UCustomLocalPlayerSubsystem::HandleTestFunction2);

	checkf(ActionTestFunction3, TEXT("[LOCAL PLAYER SUBSYSTEM] Missing 'TestFunction3' Action"));
	enhancedInputComponent->BindAction(ActionTestFunction3, ETriggerEvent::Triggered, this, &UCustomLocalPlayerSubsystem::HandleTestFunction3);
}