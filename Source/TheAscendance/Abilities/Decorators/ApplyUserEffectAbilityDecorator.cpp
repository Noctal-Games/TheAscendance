// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplyUserEffectAbilityDecorator.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/GameplayTagHelpers.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Characters/Interfaces/Susceptible.h"
#include "TheAscendance/Effects/CoreEffect.h"

void UApplyUserEffectAbilityDecorator::TriggerAbility()
{
	m_DecoratedAbility->TriggerAbility();

	if (m_ModifierData == nullptr || m_ModifierData->EffectTag.IsValid() == false)
	{
		LOG_ERROR("[APPLY USER EFFECT ABILITY DECORATOR] ApplyUserEffectAbilityModifierData was invalid or the EffectTag was invalid");
		return;
	}

	ISusceptible* target = nullptr;

	if (AActor* owner = GetAbilityOwner())
	{
		target = Cast<ISusceptible>(owner);
	}

	if (target == nullptr)
	{
		LOG_ERROR("[APPLY USER EFFECT ABILITY DECORATOR] Tried to apply UserEffect to invalid Owner");
		return;
	}

	if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
	{
		UBaseEffect* casterEffect = gameMode->CreateEffectFromTag(m_ModifierData->EffectTag);

		if (casterEffect == nullptr)
		{
			return;
		}

		if (UCoreEffect* coreEffect = Cast<UCoreEffect>(casterEffect))
		{
			target->AddEffect(coreEffect);
		}
	}
}
