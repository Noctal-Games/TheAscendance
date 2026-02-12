// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatsComponent.h"
#include "TheAscendance/Core/CoreMacros.h"

// Sets default values for this component's properties
UCharacterStatsComponent::UCharacterStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCharacterStatsComponent::Init()
{
	AddStat(ECharacterStat::SHIELD, 0);

	AddStat(ECharacterStat::HEALTH, m_BaseHealth);
	AddStat(ECharacterStat::STAMINA, m_BaseStamina);
	AddStat(ECharacterStat::MANA, m_BaseMana);

	AddStat(ECharacterStat::WALK_SPEED, m_BaseWalkSpeed);
	AddStat(ECharacterStat::SPRINT_SPEED_BONUS, m_SprintSpeedBonus);
	AddStat(ECharacterStat::CROUCH_SPEED_PENALTY, m_CrouchSpeedPenalty);

	AddStat(ECharacterStat::PHYSICAL_ATTACK, 0);
	AddStat(ECharacterStat::PHYSICAL_RESISTANCE, 0);
	AddStat(ECharacterStat::MAGIC_ATTACK, 0);
	AddStat(ECharacterStat::MAGIC_RESISTANCE, 0);
}

void UCharacterStatsComponent::AddStat(ECharacterStat stat, float maxValue)
{
	if (m_Stats.Contains(stat) == true)
	{
		m_Stats[stat] = maxValue;
	}
	else
	{
		m_Stats.Add(stat, maxValue);
	}

	if (m_StatsBase.Contains(stat) == true)
	{
		m_StatsBase[stat] = maxValue;
	}
	else
	{
		m_StatsBase.Add(stat, maxValue);
	}

	if (stat != ECharacterStat::HEALTH && stat != ECharacterStat::STAMINA && stat != ECharacterStat::MANA && stat != ECharacterStat::WALK_SPEED && stat != ECharacterStat::SHIELD)
	{
		return;
	}

	if (stat != ECharacterStat::WALK_SPEED && stat != ECharacterStat::SHIELD)
	{
		if (m_StatsMax.Contains(stat) == true)
		{
			m_StatsMax[stat] = maxValue;
		}
		else
		{
			m_StatsMax.Add(stat, maxValue);
		}
	}

	ExecuteBindings(stat);
}

void UCharacterStatsComponent::SetStat(ECharacterStat stat, float amount)
{
	if (m_Stats.Contains(stat) == false)
	{
		LogStatError(stat);
		return;
	}

	m_StatsBase[stat] = amount;
	m_Stats[stat] = amount;

	if (stat != ECharacterStat::HEALTH && stat != ECharacterStat::STAMINA && stat != ECharacterStat::MANA && stat != ECharacterStat::WALK_SPEED && stat != ECharacterStat::SHIELD)
	{
		return;
	}

	if (stat != ECharacterStat::WALK_SPEED && stat != ECharacterStat::SHIELD)
	{
		m_StatsMax[stat] = amount;
	}

	ExecuteBindings(stat);
}

void UCharacterStatsComponent::AdjustStatByValue(ECharacterStat stat, float amount)
{
	if (m_Stats.Contains(stat) == false)
	{
		LogStatError(stat);
		return;
	}

	LOG_ONSCREEN(-1, 3.0f, FColor::Green, "[CHARACTER STATS COMPONENT] Stat: %s changed from %.0f TO %.0f", *UEnum::GetValueAsString(stat), m_Stats[stat], m_Stats[stat] + amount);

	if (stat != ECharacterStat::HEALTH && stat != ECharacterStat::STAMINA && stat != ECharacterStat::MANA)
	{
		m_Stats[stat] += amount;

		if (m_Stats[stat] < 0)
		{
			m_Stats[stat] = 0;
		}

		if (stat == ECharacterStat::WALK_SPEED || stat == ECharacterStat::SHIELD)
		{
			ExecuteBindings(stat);
		}

		return;
	}

	m_Stats[stat] = FMath::Clamp(m_Stats[stat] += amount, 0, m_StatsMax[stat]);

	ExecuteBindings(stat);

	if (stat == ECharacterStat::STAMINA)
	{
		m_StaminaRegenTimer = m_StaminaRegenDelay;
	}
	else if (stat == ECharacterStat::MANA)
	{
		m_ManaRegenTimer = m_ManaRegenDelay;
	}
}

void UCharacterStatsComponent::AdjustStatByPercentage(ECharacterStat stat, float percentage)
{
	if (m_Stats.Contains(stat) == false || m_StatsBase.Contains(stat) == false)
	{
		LogStatError(stat);
		return;
	}

	float adjust = m_StatsBase[stat];

	//If percentage is negative, then get it's absolute multiplier and negate it from health. Else, add the multiplier to health.
	if (percentage < 0)
	{
		adjust *= (FMath::Abs(percentage) / 100);
		m_Stats[stat] -= FMath::FloorToInt(adjust);
	}
	else
	{
		adjust *= percentage / 100;
		m_Stats[stat] += FMath::FloorToInt(adjust);
	}

	if (stat != ECharacterStat::HEALTH && stat != ECharacterStat::STAMINA && stat != ECharacterStat::MANA && stat != ECharacterStat::WALK_SPEED)
	{
		return;
	}

	ExecuteBindings(stat);

	if (stat == ECharacterStat::STAMINA)
	{
		m_StaminaRegenTimer = m_StaminaRegenDelay;
	}
	else if (stat == ECharacterStat::MANA)
	{
		m_ManaRegenTimer = m_ManaRegenDelay;
	}
}

void UCharacterStatsComponent::AdjustMaxStatByValue(ECharacterStat stat, int amount)
{
	if (stat != ECharacterStat::HEALTH && stat != ECharacterStat::STAMINA && stat != ECharacterStat::MANA)
	{
		return;
	}

	if (m_StatsMax.Contains(stat) == false)
	{
		LogStatError(stat);
		return;
	}

	m_StatsMax[stat] += amount;
	AdjustStatByValue(stat, 0);

	ExecuteBindings(stat);
}

int UCharacterStatsComponent::GetStatAsValue(ECharacterStat stat) const
{
	if (m_Stats.Contains(stat) == false)
	{
		return 0;
	}

	return FMath::FloorToInt(m_Stats[stat]);
}

int UCharacterStatsComponent::GetStatAsPercentage(ECharacterStat stat) const
{
	if (m_Stats.Contains(stat) == false || m_StatsMax.Contains(stat) == false)
	{
		return 0;
	}

	return FMath::FloorToInt((m_Stats[stat] / m_StatsMax[stat]) * 100);
}

int UCharacterStatsComponent::GetStatBaseValue(ECharacterStat stat) const
{
	if (m_StatsBase.Contains(stat) == false)
	{
		return 0;
	}

	return m_StatsBase[stat];
}

int UCharacterStatsComponent::GetStatMaxValue(ECharacterStat stat) const
{
	if (m_StatsMax.Contains(stat) == false)
	{
		return 0;
	}

	return m_StatsMax[stat];
}

// Called every frame
void UCharacterStatsComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	if (m_Stats.Contains(ECharacterStat::STAMINA))
	{
		if (m_StaminaRegenTimer > 0)
		{
			m_StaminaRegenTimer -= deltaTime;
		}
		else
		{
			m_Stats[ECharacterStat::STAMINA] = FMath::Clamp(m_Stats[ECharacterStat::STAMINA] += m_StaminaRegenPerTick * deltaTime, 0, m_StatsMax[ECharacterStat::STAMINA]);
			ExecuteBindings(ECharacterStat::STAMINA);
		}
	}

	if (m_Stats.Contains(ECharacterStat::MANA))
	{
		if (m_ManaRegenTimer > 0)
		{
			m_ManaRegenTimer -= deltaTime;
		}
		else
		{
			m_Stats[ECharacterStat::MANA] = FMath::Clamp(m_Stats[ECharacterStat::MANA] += m_ManaRegenPerTick * deltaTime, 0, m_StatsMax[ECharacterStat::MANA]);
			ExecuteBindings(ECharacterStat::MANA);
		}
	}
}

// Called when the game starts
void UCharacterStatsComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCharacterStatsComponent::ExecuteBindings(ECharacterStat stat)
{
	const float current = m_Stats[stat];
	float max;

	if (m_StatsMax.Contains(stat) == true)
	{
		max = m_StatsMax[stat];
	}
	else
	{
		if (stat == ECharacterStat::SHIELD)
		{
			max = m_StatsMax.Contains(ECharacterStat::HEALTH) ? m_StatsMax[ECharacterStat::HEALTH] : 0.0f;
		}
	}

	OnStatChanged.Broadcast(stat, current, max);
}

void UCharacterStatsComponent::LogStatWarning(ECharacterStat stat)
{
	LOG_WARNING("[CHARACTER STATS COMPONENT] %s HAS NO BASE_%s", *GetOwner()->GetName(), *UEnum::GetValueAsString(stat));
}

void UCharacterStatsComponent::LogStatError(ECharacterStat stat)
{
	LOG_ERROR("[CHARACTER STATS COMPONENT] %s HAS NO %s STAT", *GetOwner()->GetName(), *UEnum::GetValueAsString(stat));
}




