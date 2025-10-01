// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplyEffectSpellDecorator.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/GameplayTagHelpers.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Characters/Interfaces/Susceptible.h"
#include "TheAscendance/Spells/Structs/SpellModifierData.h"
#include "TheAscendance/Effects/CoreEffect.h"
#include "TheAscendance/Effects/DeliveryEffects/ChainDeliveryEffect.h"
#include "TheAscendance/Effects/Structs/EffectData.h"

void UApplyEffectSpellDecorator::ApplyEffects(AActor* hitActor)
{
	m_DecoratedSpell->ApplyEffects(hitActor);

	if (m_ModifierData == nullptr)
	{
		LOG_ERROR("ApplyEffectSpellDecorator is missing ModifierData");
		return;
	}

	if (m_ModifierData->HasRNG == true && (FMath::FRandRange(0.0f, 100.0f) > m_ModifierData->ChanceToApply))
	{
		return;
	}

	ISusceptible* target = Cast<ISusceptible>(hitActor);

	if (target == nullptr)
	{
		LOG_ERROR("Tried to apply Effect to invalid Target");
		return;
	}

	UBaseEffect* effect = nullptr;

	if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
	{
		effect = gameMode->CreateEffectFromTag(m_ModifierData->EffectTag);
	}
	else
	{
		LOG_ERROR("Tried to apply Effect in invalid Gamemode");
		return;
	}

	if (effect == nullptr)
	{
		return;
	}

	if (UEffectData* effectData = effect->GetEffectData())
	{
		FString subType = UGameplayTagHelpers::GetTagSubtype(effectData->EffectTag);

		if (subType == "AOE")
		{

		}
		else if (subType == "Chain")
		{
			if (UChainDeliveryEffect* chainEffect = Cast<UChainDeliveryEffect>(effect))
			{
				chainEffect->Root();
				chainEffect->StartEffect(target);
			}
		}
		else
		{
			if (UCoreEffect* coreEffect = Cast<UCoreEffect>(effect))
			{
				target->AddEffect(coreEffect);
			}
			else
			{
				LOG_ERROR("Tried to apply an Effect with no subtype but failed to Cast to CoreEffect");
			}
		}
	}
	else
	{
		LOG_ERROR("Tried to apply Effect but failed to get the EffectData");
	}
}
