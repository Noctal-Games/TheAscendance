// Fill out your copyright notice in the Description page of Project Settings.


#include "TrailProjectileDecorator.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/GameplayTagHelpers.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Spells/Structs/SpellModifierData.h"
#include "TheAscendance/Effects/CoreEffect.h"
#include "TheAscendance/Effects/DeliveryEffects/AOEDeliveryEffect.h"
#include "TheAscendance/Effects/Structs/EffectData.h"

void UTrailProjectileDecorator::HandleOnUpdate(float deltaTime)
{
	if (m_TrailEffectData == nullptr)
	{
		LOG_WARNING("TrailEffectData is invalid");
		return;
	}

	FVector location = GetProjectileLocation();

	if ((location - m_LastLocation).Length() >= m_SpawnInterval)
	{
		if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
		{
			if (UAOEDeliveryEffect* effect = Cast<UAOEDeliveryEffect>(gameMode->CreateEffectFromEffectData(m_TrailEffectData.Get())))
			{
				effect->SetIgnoredActor(GetProjectileActor());
				effect->StartEffect(nullptr, location);
			}
			else
			{
				LOG_ERROR("Failed to spawn AOE Effect for Trail");
			}
		}
		else
		{
			LOG_ERROR("Tried to apply Effect in invalid Gamemode");
		}

		m_LastLocation = location;
	}
}

void UTrailProjectileDecorator::Init()
{
	if (m_EffectTag.IsValid() == false)
	{
		LOG_ERROR("Tried to create a Trail but the EffectTag wasn't set");
		return;
	}

	FString subType = UGameplayTagHelpers::GetTagSubtype(m_EffectTag);

	if (subType != "AOE")
	{
		LOG_WARNING("Tried to create a Trail with an invalid Effect");
		return;
	}

	if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
	{
		if (UEffectData* effectData = gameMode->LoadEffectData(m_EffectTag))
		{
			if (UAreaOfEffectDeliveryEffectData* aoeEffectData = Cast<UAreaOfEffectDeliveryEffectData>(effectData))
			{
				m_TrailEffectData = aoeEffectData;
				m_LastLocation = GetProjectileLocation();
				m_SpawnInterval = m_TrailEffectData->DeliveryRange * 0.5f;
			}
			else
			{
				LOG_WARNING("Tried to create a Trail with an invalid Effect");
			}
		}
		else
		{
			LOG_ERROR("Tried to create Trail but failed to get the EffectData");
		}
	}
	else
	{
		LOG_ERROR("Tried to create Trail in invalid Gamemode");
	}
}
