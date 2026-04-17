// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityFactory.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "BaseAbility.h"
#include "Structs/AbilityData.h"
#include "Interfaces/Ability.h"
#include "Components/AbilityComponent.h"
#include "Decorators/ChargedAbilityDecorator.h"

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

	for (const auto modifier : abilityData->AbilityModifiers)
	{
		if (modifier.IsValid() == false)
		{
			continue;
		}

		switch (modifier.Get().ModifierType)
		{
			case EAbilityModifierType::CHARGED:
			{
				if (modifier.GetScriptStruct() != FChargedAbilityModifier::StaticStruct())
				{
					LOG_ERROR("[ABILITY FACTORY] An AbilityModifierType struct with type CHARGED isn't of type ChargedAbilityModifier");
					continue;
				}

				const FChargedAbilityModifier& modifierData = modifier.Get<FChargedAbilityModifier>();
				ability = UChargedAbilityDecorator::Builder(ability.GetInterface(), modifierData).Build()->_getUObject();

				break;
			}
		}
	}

	ability->SetDecoratedSelf(ability.GetInterface());
	return ability.GetInterface();
}
