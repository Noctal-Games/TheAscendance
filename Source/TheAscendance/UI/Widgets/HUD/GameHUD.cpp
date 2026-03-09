// Fill out your copyright notice in the Description page of Project Settings.


#include "GameHUD.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/Subsystems/UIManagerSubsystem.h"
#include "TheAscendance/UI/Widgets/Grimoire/Grimoire.h"

#include "Widgets\CommonActivatableWidgetContainer.h"

void UGameHUD::PushGrimoireWidget()
{
	if (UUIManagerSubsystem* uiManager = UCoreFunctionLibrary::GetUIManagerSubsystem())
	{
		HUDStack->AddWidget(GrimoireClassRef);
	}
}
