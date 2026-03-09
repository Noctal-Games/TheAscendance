// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellCasterComponent.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Spells/Interfaces/Spell.h"
#include "TheAscendance/Spells/Interfaces/SpellCaster.h"
#include "TheAscendance/Spells/Structs/SpellData.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"

// Sets default values for this component's properties
USpellCasterComponent::USpellCasterComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	m_Spells.SetNum(MaxSpells);
}

void USpellCasterComponent::SetSpells(const TArray<FGameplayTag>& spellTags)
{
	if (m_SpellOwner == nullptr)
	{
		LOG_ERROR("[SPELL CASTER COMPONENT] SetSpells was called but SpellOwner is invalid");
		return;
	}

	check(m_Spells.Num() == MaxSpells);

	int maxCount = FMath::Min(spellTags.Num(), MaxSpells);

	for (int i = 0; i < maxCount; i++)
	{
		if (spellTags[i].IsValid() == false)
		{
			if (m_Spells[i] != nullptr)
			{
				m_Spells[i] = nullptr;
			}

			continue;
		}

		if (m_Spells.IsValidIndex(i) == false)
		{
			LOG_ERROR("[SPELL CASTER COMPONENT] SetSpells iterator went out of valid range");
			return;
		}

		if (ISpell* spell = m_Spells[i].GetInterface())
		{
			USpellData* spellData = spell->GetSpellData();

			if (spellData != nullptr && spellData->SpellTag == spellTags[i])
			{
				LOG_ONSCREEN(-1, 5.0f, FColor::Yellow, "NO CHANGE");
				//No Change
				continue;
			}
		}

		//Below failure checks are already logged internally
		if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
		{
			if (ISpell* spell = gameMode->CreateSpellFromTag(spellTags[i], m_SpellOwner.GetInterface()))
			{
				m_Spells[i] = spell->_getUObject();
			}
		}
	}
}

bool USpellCasterComponent::CastSpell(int slot) const
{
	if (slot - 1 < 0 || slot > m_Spells.Num())
	{
		LOG_ERROR("[SPELL CASTER COMPONENT] Tried to CastSpell from a slot that it out of range");
		return false;
	}

	if (m_Spells[slot - 1] == nullptr)
	{
		LOG_WARNING("[SPELL CASTER COMPONENT] Tried to CastSpell, but the spell slot was empty (not an error if this is expected)");
		return false;
	}

	if (m_Spells[slot - 1]->CanCast() == false)
	{
		return false;
	}

	return m_Spells[slot - 1]->CastSpell();
}

// Called when the game starts
void USpellCasterComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ISpellCaster* owner = Cast<ISpellCaster>(GetOwner()))
	{
		m_SpellOwner = owner->_getUObject();
	}
}

// Called every frame
void USpellCasterComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	for (TScriptInterface<ISpell> spell : m_Spells)
	{
		if (spell == nullptr)
		{
			continue;
		}

		spell->Update(deltaTime);
	}
}

