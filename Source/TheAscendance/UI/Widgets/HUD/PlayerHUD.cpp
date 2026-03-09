// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Characters/Player/PlayerCharacter.h"
#include "TheAscendance/UI/Elements/StatBoundProgressBar.h"
#include "ActionBar.h"

#include "Components/WidgetSwitcher.h"
#include "Components/Image.h"

void UPlayerHUD::Init(APlayerCharacter* ownerCharacter)
{
	m_OwnerCharacter = ownerCharacter;

	if (m_OwnerCharacter.IsValid() == false)
	{
		LOG_ERROR("PLAYER HUD] Attempted to Init with invalid owner character reference");
		return;
	}

	m_OwnerCharacter->m_OnInteractTargetChanged.BindUObject(this, &UPlayerHUD::UpdateCrosshair);

	m_ActionBar->Init(m_OwnerCharacter->GetLoadoutComponent());

	m_HealthBar->BoundHUDStat = EHUDBarStat::HEALTH;
	m_ManaBar->BoundHUDStat = EHUDBarStat::MANA;
	m_StaminaBar->BoundHUDStat = EHUDBarStat::STAMINA;

	BindStatBar(m_HealthBar);
	BindStatBar(m_ManaBar);
	BindStatBar(m_StaminaBar);
}

void UPlayerHUD::BindStatBar(UStatBoundProgressBar* statBar)
{
	if (statBar == nullptr)
	{
		LOG_ERROR("[PLAYER HUD] Attempted to bind null stat bar");
		return;
	}

	if (m_OwnerCharacter.IsValid() == false)
	{
		LOG_ERROR("[PLAYER HUD] Attempted to BindStatbar with invalid owner character reference");
		return;
	}

	if (UCharacterStatsComponent* statsComponent = m_OwnerCharacter->GetCharacterStatsComponent())
	{
		statBar->Init(statsComponent);
	}
	else
	{
		LOG_ERROR("[PLAYER HUD] Attempted to bind stat bar but owner character has no valid stats component");
	}
}

void UPlayerHUD::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (APlayerCharacter* player = UCoreFunctionLibrary::GetPlayerCharacter())
	{
		Init(player);
	}
}

void UPlayerHUD::UpdateCrosshair(EInteractType targetType)
{
	if (UWidget* newCrosshair = m_Crosshair->GetWidgetAtIndex((int32)targetType))
	{
		m_Crosshair->SetActiveWidget(newCrosshair);
	}
	else
	{
		LOG_ERROR("[PLAYER HUD] Tried to UpdateCrosshair but crosshair doesn't exist for: %s", *UEnum::GetValueAsString(targetType));
	}
}