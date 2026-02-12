// Fill out your copyright notice in the Description page of Project Settings.
#include "CustomPlayerController.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "PlayerCharacter.h"
#include "CustomLocalPlayerSubsystem.h"

#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ACustomPlayerController::DisplayHUD()
{
}

void ACustomPlayerController::UpdateCamera(const FVector& value)
{
	AddYawInput(value.X * HorizontalSensitivity);
	AddPitchInput(value.Y * VerticalSensitivity);
}

void ACustomPlayerController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

    if (UCustomLocalPlayerSubsystem* subsystem = GetLocalPlayer()->GetSubsystem<UCustomLocalPlayerSubsystem>())
    {
        subsystem->SetController(this);
        subsystem->SetPlayer(Cast<APlayerCharacter>(pawn));

        if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
        {
            subsystem->SetupInput(enhancedInputComponent);
        }

        subsystem->CreateHUD();
    }
    else
    {
        LOG_ERROR("[PLAYER CONTROLLER] Failed to get CustomLocalPlayerSubsystem");
    }
}

void ACustomPlayerController::OnUnPossess()
{
	Super::OnUnPossess();
}