// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManagerSubsystem.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"

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

UPlayerHUD* UUIManagerSubsystem::CreatePlayerHUD()
{
	if(m_WidgetDefaults.Contains(EWidgets::PLAYER_HUD) == false)
	{
		LOG_ERROR("[UI MANAGER] Tried to Create Player HUD, but no PlayerHUD Default was set");
		return nullptr;
	}

	TSubclassOf<UUserWidget> widgetClass = m_WidgetDefaults[EWidgets::PLAYER_HUD];

	if (widgetClass)
	{
		if(UPlayerHUD* playerHUD = CreateWidget<UPlayerHUD>(UCoreFunctionLibrary::GetGameWorld(), widgetClass))
		{
			return playerHUD;
		}
		else
		{
			LOG_ERROR("[UI MANAGER] Failed to create Player HUD widget");
		}
	}
	else
	{
		LOG_ERROR("[UI MANAGER] No valid widget class for Player HUD");
	}

	return nullptr;
}
