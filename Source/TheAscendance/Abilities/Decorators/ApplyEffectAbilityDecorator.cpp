// Fill out your copyright notice in the Description page of Project Settings.


#include "ApplyEffectAbilityDecorator.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/GameplayTagHelpers.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Characters/Interfaces/Susceptible.h"
#include "TheAscendance/Effects/CoreEffect.h"
#include "TheAscendance/Effects/DeliveryEffects/ChainDeliveryEffect.h"
#include "TheAscendance/Effects/Structs/EffectData.h"

void UApplyEffectAbilityDecorator::ApplyEffects(AActor* hitActor)
{
	m_DecoratedAbility->ApplyEffects(hitActor);

	if (m_ModifierData == nullptr)
	{
		LOG_ERROR("[APPLY EFFECT ABILITY DECORATOR] Missing ModifierData");
		return;
	}

	FString subType = UGameplayTagHelpers::GetTagSubtype(m_ModifierData->EffectTag);

	if (subType == "AOE")
	{
		LOG_WARNING("[APPLY EFFECT ABILITY DECORATOR] Tried to Apply an invalid Effect, use a SpawnEffectAbilityModifier instead");
		return;
	}

	if (m_ModifierData->HasRNG == true && (FMath::FRandRange(0.0f, 100.0f) > m_ModifierData->ChanceToApply))
	{
		return;
	}

	ISusceptible* target = Cast<ISusceptible>(hitActor);

	if (target == nullptr)
	{
		LOG_ERROR("[APPLY EFFECT ABILITY DECORATOR] Tried to apply Effect to invalid Target");
		return;
	}

	UBaseEffect* effect = nullptr;

	if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
	{
		effect = gameMode->CreateEffectFromTag(m_ModifierData->EffectTag);
	}
	else
	{
		LOG_ERROR("[APPLY EFFECT ABILITY DECORATOR] Tried to apply Effect in invalid Gamemode");
		return;
	}

	if (effect == nullptr)
	{
		return;
	}

	if (UEffectData* effectData = effect->GetEffectData())
	{
		if (subType == "Chain")
		{
			if (UChainDeliveryEffect* chainEffect = Cast<UChainDeliveryEffect>(effect))
			{
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
				LOG_ERROR("[APPLY EFFECT ABILITY DECORATOR] Tried to apply an Effect with no subtype but failed to Cast to CoreEffect");
			}
		}
	}
	else
	{
		LOG_ERROR("[APPLY EFFECT ABILITY DECORATOR] Tried to apply Effect but failed to get the EffectData");
	}
}
