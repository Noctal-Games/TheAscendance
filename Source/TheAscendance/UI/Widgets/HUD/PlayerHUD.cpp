// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Characters/Player/PlayerCharacter.h"
#include "TheAscendance/UI/Elements/StatBoundProgressBar.h"

void UPlayerHUD::Init(APlayerCharacter* ownerCharacter)
{
	m_OwnerCharacter = ownerCharacter;

	if (m_OwnerCharacter.IsValid() == false)
	{
		LOG_ERROR("PLAYER HUD] Attempted to Init with invalid owner character reference");
		return;
	}

	m_HealthBar->BoundHUDStat = EHUDBarStat::HEALTH;
	m_ManaBar->BoundHUDStat = EHUDBarStat::MANA;
	m_StaminaBar->BoundHUDStat = EHUDBarStat::STAMINA;

	BindStatBar(m_HealthBar);
	BindStatBar(m_ManaBar);
	BindStatBar(m_StaminaBar);
}

void UPlayerHUD::BindStatBar(UStatBoundProgressBar* statBar)
{
	if (m_OwnerCharacter.IsValid() == false)
	{
		LOG_ERROR("[PLAYER HUD] Attempted to BindStatbar with invalid owner character reference");
		return;
	}

	if(statBar == nullptr)
	{
		LOG_ERROR("[PLAYER HUD] Attempted to bind null stat bar");
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

void UPlayerHUD::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerHUD::NativeDestruct()
{
	Super::NativeDestruct();
}
