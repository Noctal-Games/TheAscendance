// Fill out your copyright notice in the Description page of Project Settings.


#include "OverTimeEffect.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Characters/Interfaces/Susceptible.h"
#include "Structs/EffectData.h"

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

void UOverTimeEffect::StartEffect(ISusceptible* target)
{
	UCoreEffect::StartEffect(target);

	if (m_EffectData == nullptr || target == nullptr)
	{
		return;
	}

	m_Timer = m_EffectData->Duration;
	m_IntervalTimer = m_EffectData->EffectInterval;
}

void UOverTimeEffect::Update(float deltaTime)
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

	m_Timer -= deltaTime;
	m_IntervalTimer -= deltaTime;

	if (m_IntervalTimer <= 0.0f)
	{
		if (m_Target == nullptr)
		{
			EndEffect();
			return;
		}

		m_Target->AdjustStat(m_EffectData->AffectedStat, m_EffectData->Potency);
		m_IntervalTimer += m_EffectData->EffectInterval;
	}

	if (m_Timer <= 0.0f)
	{
		EndEffect();
		return;
	}
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
