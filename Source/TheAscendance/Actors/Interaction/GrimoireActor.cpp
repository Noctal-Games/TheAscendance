// Fill out your copyright notice in the Description page of Project Settings.


#include "GrimoireActor.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/Subsystems/UIManagerSubsystem.h"
#include "TheAscendance/UI/Widgets/HUD/GameHUD.h"

void AGrimoireActor::Interact(APlayerCharacter* player)
{
	if (UUIManagerSubsystem* uiManager = UCoreFunctionLibrary::GetUIManagerSubsystem())
	{
		if (UGameHUD* gameHUD = uiManager->GetGameHUD())
		{
			gameHUD->PushGrimoireWidget();
		}
	}
}
