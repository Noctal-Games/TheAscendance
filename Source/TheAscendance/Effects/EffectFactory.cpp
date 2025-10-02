// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectFactory.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Effects/Structs/EffectData.h"
#include "TheAscendance/Effects/OverTimeEffect.h"
#include "TheAscendance/Effects/DeliveryEffects/ChainDeliveryEffect.h"
#include "TheAscendance/Effects/DeliveryEffects/AOEDeliveryEffect.h"

UBaseEffect* EffectFactory::CreateEffect(UEffectData* effectData)
{
	if (effectData == nullptr)
	{
		LOG_ERROR("Tried to create effect with invalid EffectData");
		return nullptr;
	}

	UBaseEffect* effect = nullptr;

	switch (effectData->EffectType)
	{
		case EEffectType::OVERTIME:
		{
			effect = NewObject<UOverTimeEffect>();
			break;
		}
		case EEffectType::CHAIN:
		{
			effect = NewObject<UChainDeliveryEffect>();
			break;
		}
		case EEffectType::AOE:
		{
			effect = NewObject<UAOEDeliveryEffect>();
			break;
		}
	}

	if (effect == nullptr)
	{
		LOG_ERROR("EffectFactory failed to create Effect for EffectType: %s", *UEnum::GetValueAsString(effectData->EffectType));
		return nullptr;
	}

	if (effect->Init(effectData) == false)
	{
		return nullptr;
	}

	return effect;
}
