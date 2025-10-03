// Fill out your copyright notice in the Description page of Project Settings.


#include "OverTimeEffect.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Characters/Interfaces/Susceptible.h"
#include "Structs/EffectData.h"

#include "TimerManager.h"

bool UOverTimeEffect::Init(UEffectData* effectData)
{
	if (UOverTimeEffectData* data = Cast<UOverTimeEffectData>(effectData))
	{
		m_EffectData = data;
		return true;
	}
	else
	{
		LOG_ERROR("Tried to Init OverTimeEffect with invalid EffectData");
		return false;
	}
}

void UOverTimeEffect::StartEffect(ISusceptible* target, FVector location)
{
	UCoreEffect::StartEffect(target);

	if (m_EffectData == nullptr || target == nullptr)
	{
		return;
	}

	m_Timer = m_EffectData->Duration;
	m_Interval = m_EffectData->EffectInterval;
	m_Potency = m_EffectData->Potency;

	if (m_Target->HasResistance(m_EffectData->EffectTag) == true)
	{
		m_Potency *= 0.5f;
	}

	DoEffect();

	if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		world->GetTimerManager().SetTimer(m_TimerHandle, this, &UOverTimeEffect::DoEffect, m_Interval, true);
	}
}

void UOverTimeEffect::EndEffect()
{
	if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		world->GetTimerManager().ClearTimer(m_TimerHandle);
	}

	UCoreEffect::EndEffect();
}

void UOverTimeEffect::ResetEffect()
{
	m_Timer = m_EffectData->Duration;
}

UEffectData* UOverTimeEffect::GetEffectData()
{
	if (m_EffectData.IsValid() == false)
	{
		LOG_ERROR("Effect has no valid EffectData");
		return nullptr;
	}

	return m_EffectData.Get();
}

void UOverTimeEffect::DoEffect()
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

	m_Timer -= m_Interval;

	if (m_Target == nullptr)
	{
		EndEffect();
		return;
	}

	m_Target->AdjustStat(m_EffectData->AffectedStat, m_Potency);

	if (m_Timer <= 0.0f)
	{
		EndEffect();
		return;
	}
}
