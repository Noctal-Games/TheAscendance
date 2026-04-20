// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityFactory.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "BaseAbility.h"
#include "Structs/AbilityData.h"
#include "Structs/AbilityModifierData.h"
#include "Interfaces/Ability.h"
#include "Components/AbilityComponent.h"
#include "Decorators/ChargedAbilityDecorator.h"
#include "Decorators/AOEAbilityDecorator.h"
#include "Decorators/KnockbackAbilityDecorator.h"
#include "Decorators/ApplyUserEffectAbilityDecorator.h"
#include "Decorators/ApplyEffectAbilityDecorator.h"
#include "Decorators/SpawnEffectAbilityDecorator.h"
#include "TheAscendance/Spells/Structs/SpellData.h"
#include "TheAscendance/Spells/ProjectileSpell.h"
#include "TheAscendance/Spells/LocalSpell.h"


#include "InstancedStruct.h"

IAbility* AbilityFactory::CreateAbility(UAbilityData* abilityData, UAbilityComponent* ownerComponent)
{
	if (abilityData == nullptr || ownerComponent == nullptr)
	{
		LOG_ERROR("[ABILITY FACTORY] Tried to create ability with invalid AbilityData");
		return nullptr;
	}

	TScriptInterface<IAbility> ability = nullptr;

	switch (abilityData->AbilityType)
	{
		case EAbilityType::PROJECTILE_SPELL:
		{
			ability = NewObject<UProjectileSpell>();
			break;
		}
		case EAbilityType::LOCAL_SPELL:
		{
			ability = NewObject<ULocalSpell>();
			break;
		}
		default:
		{
			LOG_ERROR("[ABILITY FACTORY] Tried to create ability with invalid AbilityType: %s", *UEnum::GetValueAsString(abilityData->AbilityType));
			return nullptr;
		}
	}

	if (ability == nullptr)
	{
		LOG_ERROR("[ABILITY FACTORY] Failed to create BaseAbility");
		return nullptr;
	}

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

			case EAbilityModifierType::AOE:
			{
				if (modifier.GetScriptStruct() != FAreaOfEffectModifier::StaticStruct())
				{
					LOG_ERROR("[ABILITY FACTORY] An AbilityModifierType struct with type AOE isn't of type AreaOfEffectModifier");
					continue;
				}

				const FAreaOfEffectModifier& modifierData = modifier.Get<FAreaOfEffectModifier>();
				ability = UAOEAbilityDecorator::Builder(ability.GetInterface(), modifierData).Build()->_getUObject();

				break;
			}

			case EAbilityModifierType::APPLY_EFFECT:
			{
				if (modifier.GetScriptStruct() != FApplyEffectModifier::StaticStruct())
				{
					LOG_ERROR("[ABILITY FACTORY] An AbilityModifierType struct with type APPLY_EFFECT isn't of type ApplyEffectModifier");
					continue;
				}

				const FApplyEffectModifier& modifierData = modifier.Get<FApplyEffectModifier>();
				ability = UApplyEffectAbilityDecorator::Builder(ability.GetInterface(), modifierData).Build()->_getUObject();

				break;
			}

			case EAbilityModifierType::KNOCKBACK:
			{
				if (modifier.GetScriptStruct() != FKnockbackAbilityModifier::StaticStruct())
				{
					LOG_ERROR("[ABILITY FACTORY] An AbilityModifierType struct with type KNOCKBACK isn't of type KnockbackAbilityModifier");
					continue;
				}

				const FKnockbackAbilityModifier& modifierData = modifier.Get<FKnockbackAbilityModifier>();
				ability = UKnockbackAbilityDecorator::Builder(ability.GetInterface(), modifierData).Build()->_getUObject();

				break;
			}

			case EAbilityModifierType::APPLY_USER_EFFECT:
			{
				if (modifier.GetScriptStruct() != FApplyUserEffectAbilityModifier::StaticStruct())
				{
					LOG_ERROR("[ABILITY FACTORY] An AbilityModifierType struct with type APPLY_USER_EFFECT isn't of type ApplyUserEffectAbilityModifier");
					continue;
				}

				const FApplyUserEffectAbilityModifier& modifierData = modifier.Get<FApplyUserEffectAbilityModifier>();
				ability = UApplyUserEffectAbilityDecorator::Builder(ability.GetInterface(), modifierData).Build()->_getUObject();

				break;
			}

			case EAbilityModifierType::SPAWN_EFFECT:
			{
				if (modifier.GetScriptStruct() != FSpawnEffectAbilityModifier::StaticStruct())
				{
					LOG_ERROR("[ABILITY FACTORY] An AbilityModifierType struct with type SPAWN_EFFECT isn't of type SpawnEffectAbilityModifier");
					continue;
				}

				const FSpawnEffectAbilityModifier& modifierData = modifier.Get<FSpawnEffectAbilityModifier>();
				ability = USpawnEffectAbilityDecorator::Builder(ability.GetInterface(), modifierData).Build()->_getUObject();

				break;
			}
		}
	}

	if (USpellData* spellData = Cast<USpellData>(abilityData))
	{
		ProcessSpellAbility(*spellData, *ability);
	}

	ability->SetDecoratedSelf(ability.GetInterface());
	ability->Init(ownerComponent, abilityData);
	return ability.GetInterface();
}

//Might not be needed if any spell-specific decorators (if any) can be handled through spell classes
void AbilityFactory::ProcessSpellAbility(const USpellData& spellData, const IAbility& ability)
{
	switch (spellData.AbilityType)
	{
		case EAbilityType::PROJECTILE_SPELL:
		{
			break;
		}
		case EAbilityType::LOCAL_SPELL:
		{
			break;
		}
		default:
		{
			LOG_ERROR("[ABILITY FACTORY] Tried to process Ability as Spell, but the AbilityType was not a SpellType");
			return;
		}
	}
}

//ISpell* AbilityFactory::CreateSpell(USpellData* spellData, ISpellCaster* spellOwner)
//{
//	if (spellData == nullptr || spellOwner == nullptr)
//	{
//		LOG_ERROR("[SPELL FACTORY] Tried to create spell with invalid SpellData or SpellOwner");
//		return nullptr;
//	}
//
//	TScriptInterface<ISpell> spell = nullptr;
//
//	//Create SpellBase for relevant type
//	switch (spellData->SpellType)
//	{
//	case ESpellType::PROJECTILE:
//	{
//		spell = NewObject<UProjectileSpell>();
//		break;
//	}
//	case ESpellType::LOCAL:
//	{
//		spell = NewObject<ULocalSpell>();
//
//		if (spellData->SpellModifiers.IsEmpty() == true)
//		{
//			LOG_ERROR("[SPELL FACTORY] Creating LocalSpell that has no SpellModifiers. The spell will do nothing.");
//		}
//		break;
//	}
//	}
//
//	if (spell == nullptr)
//	{
//		LOG_ERROR("[SPELL FACTORY] Failed to create SpellBase for SpellType: %s", *UEnum::GetValueAsString(spellData->SpellType));
//		return nullptr;
//	}
//
//	spell->Init(spellData, spellOwner);
//
//	//Create Decorators relevant to generic spell modifiers
//	for (const auto modifier : spellData->SpellModifiers)
//	{
//		if (modifier.IsValid() == false)
//		{
//			continue;
//		}
//
//		switch (modifier.Get().ModifierType)
//		{
//
//		case EGenericSpellModifierType::APPLY_CASTER_EFFECT:
//		{
//			if (modifier.GetScriptStruct() != FApplyCasterEffectModifier::StaticStruct())
//			{
//				LOG_ERROR("[SPELL FACTORY] A GenericSpellModifierType struct with type APPLY_CASTER_EFFECT isn't of type ApplyCasterEffectModifier");
//				continue;
//			}
//
//			const FApplyCasterEffectModifier& modifierData = modifier.Get<FApplyCasterEffectModifier>();
//			spell = UApplyCasterEffectSpellDecorator::Builder(spell.GetInterface(), modifierData).Build()->_getUObject();
//
//			break;
//		}
//
//		case EGenericSpellModifierType::APPLY_EFFECT:
//		{
//			if (modifier.GetScriptStruct() != FApplyEffectModifier::StaticStruct())
//			{
//				LOG_ERROR("[SPELL FACTORY] A GenericSpellModifierType struct with type APPLY_EFFECT isn't of type ApplyEffectModifier");
//				continue;
//			}
//
//			const FApplyEffectModifier& modifierData = modifier.Get<FApplyEffectModifier>();
//			spell = UApplyEffectSpellDecorator::Builder(spell.GetInterface(), modifierData).Build()->_getUObject();
//
//			break;
//		}
//
//		case EGenericSpellModifierType::SPAWN_EFFECT:
//		{
//			if (modifier.GetScriptStruct() != FSpawnEffectModifier::StaticStruct())
//			{
//				LOG_ERROR("[SPELL FACTORY] A GenericSpellModifierType struct with type SPAWN_EFFECT isn't of type SpawnEffectModifier");
//				continue;
//			}
//
//			const FSpawnEffectModifier& modifierData = modifier.Get<FSpawnEffectModifier>();
//			spell = USpawnEffectSpellDecorator::Builder(spell.GetInterface(), modifierData).Build()->_getUObject();
//
//			break;
//		}
//		}
//	}
//
//	//Create Decorators relevant to ranged spell modifiers if ranged
//	if (spellData->SpellType == ESpellType::PROJECTILE || spellData->SpellType == ESpellType::HITSCAN)
//	{
//		if (URangedSpellData* rangedData = Cast<URangedSpellData>(spellData))
//		{
//			for (const auto modifier : rangedData->RangedSpellModifiers)
//			{
//				if (modifier.IsValid() == false)
//				{
//					continue;
//				}
//
//				switch (modifier.Get().ModifierType)
//				{
//				case ERangedSpellModifierType::SCATTER:
//				{
//					break;
//				}
//
//				case ERangedSpellModifierType::KNOCKBACK:
//				{
//					if (modifier.GetScriptStruct() != FKnockbackSpellModifier::StaticStruct())
//					{
//						LOG_ERROR("[SPELL FACTORY] ARangedSpellModifierType struct with type KNOCKBACK isn't of type KnockbackSpellModifier");
//						continue;
//					}
//
//					const FKnockbackSpellModifier& modifierData = modifier.Get<FKnockbackSpellModifier>();
//					spell = UKnockbackSpellDecorator::Builder(spell.GetInterface(), modifierData).Build()->_getUObject();
//					break;
//				}
//				}
//			}
//		}
//	}
//
//	//Create Decorators relevant to specific spell type
//	switch (spellData->SpellType)
//	{
//	case ESpellType::PROJECTILE:
//	{
//		if (UProjectileSpellData* projectileData = Cast<UProjectileSpellData>(spellData))
//		{
//		}
//		break;
//	}
//	}
//
//	spell->SetDecoratedSelf(spell.GetInterface());
//	return spell.GetInterface();
//}