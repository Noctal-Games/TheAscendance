// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellFactory.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "ProjectileSpell.h"
#include "LocalSpell.h"
#include "Structs/SpellData.h"
#include "Structs/SpellModifierData.h"
#include "Interfaces/Spell.h"
#include "Interfaces/SpellCaster.h"

#include "InstancedStruct.h"

ISpell* SpellFactory::CreateSpell(USpellData* spellData, ISpellCaster* spellOwner)
{
	return nullptr;
	//if (spellData == nullptr || spellOwner == nullptr)
	//{
	//	LOG_ERROR("[SPELL FACTORY] Tried to create spell with invalid SpellData or SpellOwner");
	//	return nullptr;
	//}

	//TScriptInterface<ISpell> spell = nullptr;
	//
	////Create SpellBase for relevant type
	//switch (spellData->SpellType)
	//{
	//	case ESpellType::PROJECTILE:
	//	{
	//		spell = NewObject<UProjectileSpell>();
	//		break;
	//	}
	//	case ESpellType::LOCAL:
	//	{
	//		spell = NewObject<ULocalSpell>();

	//		if (spellData->SpellModifiers.IsEmpty() == true)
	//		{
	//			LOG_ERROR("[SPELL FACTORY] Creating LocalSpell that has no SpellModifiers. The spell will do nothing.");
	//		}
	//		break;
	//	}
	//}

	//if (spell == nullptr)
	//{
	//	LOG_ERROR("[SPELL FACTORY] Failed to create SpellBase for SpellType: %s", *UEnum::GetValueAsString(spellData->SpellType));
	//	return nullptr;
	//}

	//spell->Init(spellData, spellOwner);

	////Create Decorators relevant to generic spell modifiers
	//for (const auto modifier : spellData->SpellModifiers)
	//{
	//	if (modifier.IsValid() == false)
	//	{
	//		continue;
	//	}

	//	switch (modifier.Get().ModifierType)
	//	{
	//		case EGenericSpellModifierType::APPLY_CASTER_EFFECT:
	//		{
	//			if (modifier.GetScriptStruct() != FApplyCasterEffectModifier::StaticStruct())
	//			{
	//				LOG_ERROR("[SPELL FACTORY] A GenericSpellModifierType struct with type APPLY_CASTER_EFFECT isn't of type ApplyCasterEffectModifier");
	//				continue;
	//			}

	//			const FApplyCasterEffectModifier& modifierData = modifier.Get<FApplyCasterEffectModifier>();
	//			spell = UApplyCasterEffectSpellDecorator::Builder(spell.GetInterface(), modifierData).Build()->_getUObject();

	//			break;
	//		}

	//		case EGenericSpellModifierType::APPLY_EFFECT:
	//		{
	//			if (modifier.GetScriptStruct() != FApplyEffectModifier::StaticStruct())
	//			{
	//				LOG_ERROR("[SPELL FACTORY] A GenericSpellModifierType struct with type APPLY_EFFECT isn't of type ApplyEffectModifier");
	//				continue;
	//			}

	//			const FApplyEffectModifier& modifierData = modifier.Get<FApplyEffectModifier>();
	//			spell = UApplyEffectSpellDecorator::Builder(spell.GetInterface(), modifierData).Build()->_getUObject();

	//			break;
	//		}

	//		case EGenericSpellModifierType::SPAWN_EFFECT:
	//		{
	//			if (modifier.GetScriptStruct() != FSpawnEffectModifier::StaticStruct())
	//			{
	//				LOG_ERROR("[SPELL FACTORY] A GenericSpellModifierType struct with type SPAWN_EFFECT isn't of type SpawnEffectModifier");
	//				continue;
	//			}

	//			const FSpawnEffectModifier& modifierData = modifier.Get<FSpawnEffectModifier>();
	//			spell = USpawnEffectSpellDecorator::Builder(spell.GetInterface(), modifierData).Build()->_getUObject();

	//			break;
	//		}
	//	}
	//}

	////Create Decorators relevant to ranged spell modifiers if ranged
	//if (spellData->SpellType == ESpellType::PROJECTILE || spellData->SpellType == ESpellType::HITSCAN)
	//{
	//	if (URangedSpellData* rangedData = Cast<URangedSpellData>(spellData))
	//	{
	//		for (const auto modifier : rangedData->RangedSpellModifiers)
	//		{
	//			if (modifier.IsValid() == false)
	//			{
	//				continue;
	//			}

	//			switch (modifier.Get().ModifierType)
	//			{
	//				case ERangedSpellModifierType::SCATTER:
	//				{
	//					break;
	//				}

	//				case ERangedSpellModifierType::KNOCKBACK:
	//				{
	//					if (modifier.GetScriptStruct() != FKnockbackSpellModifier::StaticStruct())
	//					{
	//						LOG_ERROR("[SPELL FACTORY] ARangedSpellModifierType struct with type KNOCKBACK isn't of type KnockbackSpellModifier");
	//						continue;
	//					}

	//					const FKnockbackSpellModifier& modifierData = modifier.Get<FKnockbackSpellModifier>();
	//					spell = UKnockbackSpellDecorator::Builder(spell.GetInterface(), modifierData).Build()->_getUObject();
	//					break;
	//				}
	//			}
	//		}
	//	}
	//}

	////Create Decorators relevant to specific spell type
	//switch (spellData->SpellType)
	//{
	//	case ESpellType::PROJECTILE:
	//	{
	//		if (UProjectileSpellData* projectileData = Cast<UProjectileSpellData>(spellData))
	//		{
	//			/*for (const auto modifier : projectileData->ProjectileSpellModifiers)
	//			{
	//				if (modifier.IsValid() == false)
	//				{
	//					continue;
	//				}

	//				switch (modifier.Get().ModifierType)
	//				{
	//					case EProjectileSpellModifierType::TRAIL:
	//					{
	//						if (modifier.GetScriptStruct() != FTrailSpellModifier::StaticStruct())
	//						{
	//							LOG_ERROR("[SPELL FACTORY] A ProjectileSpellModifierType struct with type TRAIL isn't of type TrailSpellModifier");
	//							continue;
	//						}

	//						const FTrailSpellModifier& modifierData = modifier.Get<FTrailSpellModifier>();
	//						spell = UTrailSpellDecorator::Builder(spell.GetInterface(), modifierData).Build()->_getUObject();
	//						break;
	//					}

	//					case EProjectileSpellModifierType::PENETRATION:
	//					{
	//						if (modifier.GetScriptStruct() != FPenetrationSpellModifier::StaticStruct())
	//						{
	//							LOG_ERROR("[SPELL FACTORY] A ProjectileSpellModifierType struct with type PENETRATION isn't of type PenetrationSpellModifier");
	//							continue;
	//						}

	//						const FPenetrationSpellModifier& modifierData = modifier.Get<FPenetrationSpellModifier>();
	//						spell = UPenetrationSpellDecorator::Builder(spell.GetInterface(), modifierData).Build()->_getUObject();
	//						break;
	//					}
	//				}
	//			}*/
	//		}
	//		break;
	//	}
	//}

	//spell->SetDecoratedSelf(spell.GetInterface());
	//return spell.GetInterface();
}
