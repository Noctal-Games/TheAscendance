// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnEffectAbilityDecorator.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/GameplayTagHelpers.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Spells/Structs/SpellModifierData.h"
#include "TheAscendance/Effects/CoreEffect.h"
#include "TheAscendance/Effects/DeliveryEffects/ChainDeliveryEffect.h"
#include "TheAscendance/Effects/DeliveryEffects/AOEDeliveryEffect.h"
#include "TheAscendance/Effects/Structs/EffectData.h"

void USpawnEffectAbilityDecorator::OnHit(AActor* hitActor, const FVector& spellHitLocation)
{
	m_DecoratedAbility->OnHit(hitActor, spellHitLocation);

	if (m_ModifierData == nullptr)
	{
		return;
	}

	FString subType = UGameplayTagHelpers::GetTagSubtype(m_ModifierData->EffectTag);

	if (subType != "AOE")
	{
		LOG_WARNING("[SPAWN EFFECT ABILITY DECORATOR] Tried to Spawn an invalid Effect, use an ApplyEffectAbilityModifier or ApplyCasterEffectAbilityModifier instead");
		return;
	}

	UBaseEffect* effect = nullptr;

	if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
	{
		effect = gameMode->CreateEffectFromTag(m_ModifierData->EffectTag);
	}
	else
	{
		LOG_ERROR("[SPAWN EFFECT ABILITY DECORATOR] Tried to apply Effect in invalid Gamemode");
		return;
	}

	if (effect == nullptr)
	{
		return;
	}

	if (UEffectData* effectData = effect->GetEffectData())
	{
		if (subType == "AOE")
		{
			if (UAOEDeliveryEffect* aoeEffect = Cast<UAOEDeliveryEffect>(effect))
			{
				aoeEffect->StartEffect(nullptr, spellHitLocation);
			}
		}
	}
	else
	{
		LOG_ERROR("[SPAWN EFFECT ABILITY DECORATOR] Tried to apply Effect but failed to get the EffectData");
	}
}
