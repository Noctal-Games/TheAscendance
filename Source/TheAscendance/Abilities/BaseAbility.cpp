// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAbility.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "Structs/AbilityData.h"
#include "Components/AbilityComponent.h"

void UBaseAbility::Init(UAbilityComponent* ownerComponent, UAbilityData* abilityData)
{
	m_OwnerComponent = ownerComponent;

	if(m_OwnerComponent == nullptr)
	{
		LOG_ERROR("[BASE ABILITY] Failed to Init Ability, OwnerComponent was invalid");

	}

	m_AbilityData = abilityData;

	if (m_AbilityData = nullptr)
	{
		LOG_ERROR("[BASE ABILITY] Failed to Init Ability, AbilityData was invalid");
		return;
	}
}

void UBaseAbility::Activate()
{
}
void UBaseAbility::End()
{
}

void UBaseAbility::OnHitWindowStart()
{
}

void UBaseAbility::OnHitWindowEnd()
{
}

const FGameplayTag& UBaseAbility::GetAbilityTag() const
{
	if (m_AbilityData == nullptr)
	{
		LOG_ERROR("[BASE ABILITY] GetAbilityTag was called but AbilityData is invalid");
		return FGameplayTag::EmptyTag;
	}

	return m_AbilityData->AbilityTag;
}
