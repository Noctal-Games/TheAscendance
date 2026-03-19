// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManagerSubsystem.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/UI/Widgets/HUD/GameHUD.h"
#include "TheAscendance/Characters/Player/CustomPlayerController.h"

#include "Blueprint/UserWidget.h"

void UUIManagerSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);
}

void UUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UUIManagerSubsystem::ShouldCreateSubsystem(UObject* outer) const
{
	if (this->GetClass()->IsInBlueprint() && Super::ShouldCreateSubsystem(outer))
	{
		return true;
	}

	return false;
}

UGameHUD* UUIManagerSubsystem::GetGameHUD()
{
	return m_GameHUD;
}

UGrimoire* UUIManagerSubsystem::GetGrimoireRef()
{
	if (m_Grimoire == nullptr)
	{
		return nullptr;
	}

	return m_Grimoire.Get();
}

void UUIManagerSubsystem::SetGrimoireRef(UGrimoire* grimoire)
{
	m_Grimoire = grimoire;
}

void UUIManagerSubsystem::CreateGameHUD(ACustomPlayerController* controller)
{
	if (GameHUDDefault == nullptr)
	{
		LOG_ERROR("[UI MANAGER] GameHUD Default was invalid");
		return;
	}

	m_GameHUD = CreateWidget<UGameHUD>(controller, GameHUDDefault);
	m_GameHUD->AddToViewport();

	LOG_WARNING("[UI MANAGER] GameHUD Created");
}
