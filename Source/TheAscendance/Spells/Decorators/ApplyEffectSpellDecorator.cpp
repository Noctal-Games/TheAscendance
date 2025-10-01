// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplyEffectSpellDecorator.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/GameplayTagHelpers.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Characters/Interfaces/Susceptible.h"
#include "TheAscendance/Spells/Structs/SpellModifierData.h"
#include "TheAscendance/Effects/BaseEffect.h"

void UApplyEffectSpellDecorator::ApplyEffects(AActor* hitActor)
{
	m_DecoratedSpell->ApplyEffects(hitActor);

	if (m_ModifierData == nullptr)
	{
		LOG_ERROR("ApplyEffectSpellDecorator is missing ModifierData");
		return;
	}

	ISusceptible* target = Cast<ISusceptible>(hitActor);

	if (target == nullptr)
	{
		LOG_ERROR("Tried to apply Effect to invalid Target");
		return;
	}

	if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
	{
		if (m_ModifierData->HasRNG == false)
		{
			UBaseEffect* effect = gameMode->CreateEffectFromTag(m_ModifierData->EffectTag);

			if (effect == nullptr)
			{
				return;
			}

			target->AddEffect(effect);
			return;
		}

		float rand = FMath::FRandRange(0.0f, 100.0f);

		if (rand > m_ModifierData->ChanceToApply)
		{
			return;
		}

		UBaseEffect* effect = gameMode->CreateEffectFromTag(m_ModifierData->EffectTag);

		if (effect == nullptr)
		{
			return;
		}

		target->AddEffect(effect);
	}
}
