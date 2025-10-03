// Fill out your copyright notice in the Description page of Project Settings.


#include "DurationEffect.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Characters/Interfaces/Susceptible.h"
#include "Structs/EffectData.h"
#include "Enums/AffectableStats.h"

#include "TimerManager.h"

bool UDurationEffect::Init(UEffectData* effectData)
{
	if (UDurationEffectData* data = Cast<UDurationEffectData>(effectData))
	{
		m_EffectData = data;
		return true;
	}
	else
	{
		LOG_ERROR("Tried to Init DurationEffect with invalid EffectData");
		return false;
	}
}

void UDurationEffect::StartEffect(ISusceptible* target, FVector location)
{
	UCoreEffect::StartEffect(target);

	if (m_EffectData == nullptr || target == nullptr)
	{
		return;
	}

	m_IsActive = false;
	m_Potency = m_EffectData->Potency;

	if (m_Target->HasResistance(m_EffectData->EffectTag) == true)
	{
		m_Potency *= 0.5f;
	}

	DoEffect();

	if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		world->GetTimerManager().SetTimer(m_TimerHandle, this, &UDurationEffect::DoEffect, m_EffectData->Duration, true);
	}
}

void UDurationEffect::EndEffect()
{
	if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		world->GetTimerManager().ClearTimer(m_TimerHandle);
	}

	UCoreEffect::EndEffect();
}

void UDurationEffect::ResetEffect()
{
	if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		world->GetTimerManager().ClearTimer(m_TimerHandle);
		world->GetTimerManager().SetTimer(m_TimerHandle, this, &UDurationEffect::DoEffect, m_EffectData->Duration, true);
	}
}

UEffectData* UDurationEffect::GetEffectData()
{
	if (m_EffectData.IsValid() == false)
	{
		LOG_ERROR("Effect has no valid EffectData");
		return nullptr;
	}

	return m_EffectData.Get();
}

void UDurationEffect::DoEffect()
{
	if (m_EffectData.IsValid() == false)
	{
		LOG_ERROR("Effect has no valid EffectData");
		return;
	}

	if (m_HasEnded == true)
	{
		return;
	}

	ProcessAffectedStat();

	if (m_IsActive == true)
	{
		EndEffect();
	}

	m_IsActive = true;
}

void UDurationEffect::ProcessAffectedStat()
{
	switch (m_EffectData->AffectedStat)
	{
		case EDurationAffectableStat::MAX_HEALTH:
		{
			m_Target->AdjustMaxStat(ECharacterStat::HEALTH, m_IsActive ? (m_Potency * -1.0f) : m_Potency);
			break;
		}
		case EDurationAffectableStat::MAX_MANA: 
		{
			m_Target->AdjustMaxStat(ECharacterStat::MANA, m_IsActive ? (m_Potency * -1.0f) : m_Potency);
			break;
		}
		case EDurationAffectableStat::MAX_STAMINA: 
		{
			m_Target->AdjustMaxStat(ECharacterStat::STAMINA, m_IsActive ? (m_Potency * -1.0f) : m_Potency);
			break;
		}
		case EDurationAffectableStat::WALK_SPEED:
		{
			m_Target->AdjustStat(ECharacterStat::WALK_SPEED, m_IsActive ? (m_Potency * -1.0f) : m_Potency);
			m_Target->AdjustStat(ECharacterStat::SPRINT_SPEED_BONUS, m_IsActive ? (m_Potency * -1.0f) : m_Potency);
			break;
		}
		case EDurationAffectableStat::SHIELD:
		{
			m_Target->AdjustStat(ECharacterStat::SHIELD, m_IsActive ? (m_Potency * -1.0f) : m_Potency);
			break;
		}
		case EDurationAffectableStat::PHYSICAL_ATTACK:
		{
			m_Target->AdjustStat(ECharacterStat::PHYSICAL_ATTACK, m_IsActive ? (m_Potency * -1.0f) : m_Potency);
			break;
		}
		case EDurationAffectableStat::MAGIC_ATTACK:
		{
			m_Target->AdjustStat(ECharacterStat::MAGIC_ATTACK, m_IsActive ? (m_Potency * -1.0f) : m_Potency);
			break;
		}
		case EDurationAffectableStat::PHYSICAL_RESISTANCE:
		{
			m_Target->AdjustStat(ECharacterStat::PHYSICAL_RESISTANCE, m_IsActive ? (m_Potency * -1.0f) : m_Potency);
			break;
		}
		case EDurationAffectableStat::MAGIC_RESISTANCE:
		{
			m_Target->AdjustStat(ECharacterStat::MAGIC_RESISTANCE, m_IsActive ? (m_Potency * -1.0f) : m_Potency);
			break;
		}
	}
}
