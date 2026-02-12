// Fill out your copyright notice in the Description page of Project Settings.


#include "StatBoundProgressBar.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Characters/Components/CharacterStatsComponent.h"

void UStatBoundProgressBar::Init(UCharacterStatsComponent* statsComponent)
{
	if (m_OwnerStatsComponent == statsComponent)
	{
		return;
	}

	UnbindFromStat();
	m_OwnerStatsComponent = statsComponent;
	BindToStat();
}

void UStatBoundProgressBar::BindToStat()
{
	if (m_BoundStatDelegateHandle.IsValid())
	{
		return;
	}

	if (m_OwnerStatsComponent.IsValid() == false)
	{
		LOG_ERROR("[STAT BOUND PROGRESS BAR] Attempting to bind to stat with invalid stats component reference");
		return;
	}

	if(BoundHUDStat == EHUDBarStat::NONE)
	{
		LOG_ERROR("[STAT BOUND PROGRESS BAR] Attempting to bind to stat but BoundStat isn't set");
		return;
	}
		
	m_BoundStatDelegateHandle = m_OwnerStatsComponent->OnStatChanged.AddUObject(this, &UStatBoundProgressBar::HandleStatChanged);

	m_BoundStat = GetCharacterStatFromHUDStat(BoundHUDStat);

	const float current = m_OwnerStatsComponent->GetStatAsValue(m_BoundStat);
	const float max = m_OwnerStatsComponent->GetStatMaxValue(m_BoundStat);

	HandleStatChanged(m_BoundStat, current, max);
}

void UStatBoundProgressBar::UnbindFromStat()
{
	if(m_BoundStatDelegateHandle.IsValid() == false)
	{
		return;
	}

	if (m_OwnerStatsComponent.IsValid())
	{
		m_OwnerStatsComponent->OnStatChanged.Remove(m_BoundStatDelegateHandle);
	}

	m_BoundStatDelegateHandle.Reset();
}

void UStatBoundProgressBar::HandleStatChanged(ECharacterStat stat, float current, float max)
{
	if(stat != m_BoundStat)
	{
		return;
	}

	if (max <= 0.0f)
	{
		SetPercent(0.0f);
		return;
	}

	SetPercent(current / max);
}

void UStatBoundProgressBar::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
}

void UStatBoundProgressBar::ReleaseSlateResources(bool bReleaseChildren)
{
	UnbindFromStat();

	Super::ReleaseSlateResources(bReleaseChildren);
}

ECharacterStat UStatBoundProgressBar::GetCharacterStatFromHUDStat(EHUDBarStat stat)
{
	switch (stat)
	{
		case EHUDBarStat::HEALTH:  return ECharacterStat::HEALTH;
		case EHUDBarStat::STAMINA: return ECharacterStat::STAMINA;
		case EHUDBarStat::MANA:    return ECharacterStat::MANA;
		case EHUDBarStat::SHIELD:  return ECharacterStat::SHIELD;
		default:				   return ECharacterStat::NONE;
	}
}
