// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplyCasterEffectSpellDecorator.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/GameplayTagHelpers.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Spells/Interfaces/SpellCaster.h"
#include "TheAscendance/Characters/Interfaces/Susceptible.h"
#include "TheAscendance/Effects/BaseEffect.h"

bool UApplyCasterEffectSpellDecorator::CastSpell()
{
	if (m_DecoratedSpell->CastSpell() == false)
	{
		return false;
	}

	if (m_ModifierData == nullptr || m_ModifierData->EffectTag.IsValid() == false)
	{
		LOG_ERROR("ApplyCasterEffectModiferData was invalid or the EffectTag was invalid");
		return true;
	}

	ISusceptible* target = nullptr;

	if (ISpellCaster* caster = GetSpellOwner())
	{
		target = Cast<ISusceptible>(caster->GetSpellOwner());
	}

	if (target == nullptr)
	{
		LOG_ERROR("Tried to apply CasterEffect to invalid Caster");
		return true;
	}

	if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
	{
		UBaseEffect* casterEffect = gameMode->CreateEffectFromTag(m_ModifierData->EffectTag);

		if (casterEffect == nullptr)
		{
			return true;
		}

		target->AddEffect(casterEffect);
	}

	return true;
}
