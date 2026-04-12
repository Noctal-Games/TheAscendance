// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityFactory.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "BaseAbility.h"
#include "Structs/AbilityData.h"
#include "Interfaces/Ability.h"
#include "Components/AbilityComponent.h"

#include "InstancedStruct.h"

IAbility* AbilityFactory::CreateAbility(UAbilityData* abilityData, UAbilityComponent* ownerComponent)
{
	if (abilityData == nullptr || ownerComponent == nullptr)
	{
		LOG_ERROR("[ABILITY FACTORY] Tried to create ability with invalid AbilityData");
		return nullptr;
	}

	TScriptInterface<IAbility> ability = NewObject<UBaseAbility>();

	if (ability == nullptr)
	{
		LOG_ERROR("[ABILITY FACTORY] Failed to create BaseAbility");
		return nullptr;
	}

	ability->Init(ownerComponent, abilityData);
	return ability.GetInterface();
}
