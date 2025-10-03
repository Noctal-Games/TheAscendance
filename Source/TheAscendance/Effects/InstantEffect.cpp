// Fill out your copyright notice in the Description page of Project Settings.
#include "InstantEffect.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Characters/Interfaces/Susceptible.h"
#include "Structs/EffectData.h"
#include "Enums/AffectableStats.h"

bool UInstantEffect::Init(UEffectData* effectData)
{
	if (UInstantEffectData* data = Cast<UInstantEffectData>(effectData))
	{
		m_EffectData = data;
		return true;
	}
	else
	{
		LOG_ERROR("Tried to Init InstantEffect with invalid EffectData");
		return false;
	}
}

void UInstantEffect::StartEffect(ISusceptible* target, FVector location)
{
	UCoreEffect::StartEffect(target);

	if (m_EffectData == nullptr || target == nullptr)
	{
		return;
	}

	m_Potency = m_EffectData->Potency;

	if (m_Target->HasResistance(m_EffectData->EffectTag) == true)
	{
		m_Potency *= 0.5f;
	}

	DoEffect();
}

UEffectData* UInstantEffect::GetEffectData()
{
	if (m_EffectData.IsValid() == false)
	{
		LOG_ERROR("Effect has no valid EffectData");
		return nullptr;
	}

	return m_EffectData.Get();
}

void UInstantEffect::DoEffect()
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
}

void UInstantEffect::ProcessAffectedStat()
{
	switch (m_EffectData->AffectedStat)
	{
		case EInstantAffectableStat::HEALTH:
		{
			m_Target->AdjustStat(ECharacterStat::HEALTH, m_Potency);
			break;
		}
		case EInstantAffectableStat::MANA:
		{
			m_Target->AdjustStat(ECharacterStat::MANA, m_Potency);
			break;
		}
		case EInstantAffectableStat::STAMINA:
		{
			m_Target->AdjustStat(ECharacterStat::STAMINA, m_Potency);
			break;
		}
		case EInstantAffectableStat::SHIELD:
		{
			m_Target->AdjustStat(ECharacterStat::SHIELD, m_Potency);
			break;
		}
	}
}
