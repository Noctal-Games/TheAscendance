// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellFactory.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "ProjectileSpell.h"
#include "Structs/SpellData.h"
#include "Structs/SpellModifierData.h"
#include "Interfaces/Spell.h"
#include "Interfaces/SpellCaster.h"
#include "Decorators/AOESpellDecorator.h"
#include "Decorators/KnockbackSpellDecorator.h"
#include "Decorators/PenetrationSpellDecorator.h"

#include "InstancedStruct.h"

ISpell* SpellFactory::CreateSpell(USpellData* spellData, ISpellCaster* spellOwner)
{
	if (spellData == nullptr || spellOwner == nullptr)
	{
		LOG_ERROR("Tried to create spell with invalid SpellData or SpellOwner");
		return nullptr;
	}

	TScriptInterface<ISpell> spell = nullptr;
	
	//Create SpellBase for relevant type
	switch (spellData->SpellType)
	{
		case ESpellType::PROJECTILE:
		{
			spell = NewObject<UProjectileSpell>();
			break;
		}
	}

	if (spell == nullptr)
	{
		LOG_ERROR("Failed to create SpellBase for SpellType: %s", *UEnum::GetValueAsString(spellData->SpellType));
		return nullptr;
	}

	spell->Init(spellData, spellOwner);

	//Create Decorators relevant to generic spell modifiers
	for (const auto modifier : spellData->SpellModifiers)
	{
		if (modifier.IsValid() == false)
		{
			continue;
		}

		switch (modifier.Get().ModifierType)
		{
			case EGenericSpellModifierType::AOE:
			{
				if (modifier.GetScriptStruct() != FAreaOfEffectModifier::StaticStruct())
				{
					LOG_ERROR("A GenericSpellModifierType struct with type AOE isn't of type AreaOfEffectModifier");
					continue;
				}

				const FAreaOfEffectModifier& aoeModifier = modifier.Get<FAreaOfEffectModifier>();
				spell = UAOESpellDecorator::Builder(spell.GetInterface(), aoeModifier).Build()->_getUObject();

				break;
			}

			case EGenericSpellModifierType::APPLY_CASTER_EFFECT:
			{
				break;
			}
		}
	}

	//Create Decorators relevant to ranged spell modifiers if ranged
	if (spellData->SpellType == ESpellType::PROJECTILE || spellData->SpellType == ESpellType::HITSCAN)
	{
		if (URangedSpellData* rangedData = Cast<URangedSpellData>(spellData))
		{
			for (const auto modifier : rangedData->RangedSpellModifiers)
			{
				if (modifier.IsValid() == false)
				{
					continue;
				}

				switch (modifier.Get().ModifierType)
				{
					case ERangedSpellModifierType::SCATTER:
					{
						break;
					}

					case ERangedSpellModifierType::KNOCKBACK:
					{
						spell = UKnockbackSpellDecorator::Builder(spell.GetInterface()).Build()->_getUObject();
						break;
					}
				}
			}
		}
	}

	//Create Decorators relevant to specific spell type
	switch (spellData->SpellType)
	{
		case ESpellType::PROJECTILE:
		{
			if (UProjectileSpellData* projectileData = Cast<UProjectileSpellData>(spellData))
			{
				for (const auto modifier : projectileData->ProjectileSpellModifiers)
				{
					if (modifier.IsValid() == false)
					{
						continue;
					}

					switch (modifier.Get().ModifierType)
					{
						case EProjectileSpellModifierType::TRAIL:
						{
							break;
						}

						case EProjectileSpellModifierType::PENETRATION:
						{
							if (modifier.GetScriptStruct() != FPenetrationSpellModifier::StaticStruct())
							{
								LOG_ERROR("A ProjectileSpellModifierType struct with type PENETRATION isn't of type PenetrationSpellModifier");
								continue;
							}

							const FPenetrationSpellModifier& penetrationModifier = modifier.Get<FPenetrationSpellModifier>();
							spell = UPenetrationSpellDecorator::Builder(spell.GetInterface(), penetrationModifier).Build()->_getUObject();
							break;
						}
					}
				}
			}
			break;
		}
	}

	spell->SetDecoratedSelf(spell.GetInterface());
	return spell.GetInterface();
}
