// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalSpell.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "Structs/SpellData.h"

//void ULocalSpell::Init(USpellData* spellData, ISpellCaster* spellOwner)
//{
//	UBaseSpell::Init(spellData, spellOwner);
//
//	if (spellData == nullptr || spellOwner == nullptr)
//	{
//		return;
//	}
//
//	m_SpellData = Cast<ULocalSpellData>(spellData);
//
//	if (m_SpellData == nullptr)
//	{
//		LOG_ERROR("[LOCAL SPELL] Tried to Init LocalSpellBase with invalid SpellData");
//	}
//}

void ULocalSpell::Execute()
{
	if (UBaseSpell::CanStart() == false)
	{
		return;
	}

	if (AActor* owner = GetAbilityOwner())
	{
		FVector ownerLocation = owner->GetActorLocation();

		//Process hit event, nothing will happen if hit-related events are not used in LocalSpellData.
		//m_DecoratedSelf->LoadHitNiagara();

		m_DecoratedSelf->OnHit(nullptr, ownerLocation);
		m_DecoratedSelf->ProcessHit(ownerLocation);
	}
}

void ULocalSpell::ProcessHit(const FVector& spellHitLocation)
{
	if (AActor* owner = GetAbilityOwner())
	{
		if (m_HitActors.Contains(owner) == true)
		{
			m_HitActors.Remove(owner);
		}
	}

	UBaseSpell::ProcessHit(spellHitLocation);
}

//USpellData* ULocalSpell::GetSpellData()
//{
//	if (m_SpellData.IsValid() == false)
//	{
//		LOG_ERROR("[LOCAL SPELL] Tried to GetSpellData for LocalSpell but SpellData is invalid");
//		return nullptr;
//	}
//
//	return m_SpellData.Get();
//}
