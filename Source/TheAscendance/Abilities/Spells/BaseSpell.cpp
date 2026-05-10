// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseSpell.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Abilities/Interfaces/Ability.h"
#include "Structs/SpellData.h"
#include "TheAscendance/Abilities/Components/AbilityComponent.h"

//void UBaseSpell::Init(USpellData* spellData, ISpellCaster* spellOwner)
//{
//	if (spellData == nullptr || spellOwner == nullptr)
//	{
//		LOG_ERROR("[BASE SPELL] Tried to Init spell with invalid SpellData or SpellOwner");
//		return;
//	}
//
//	m_SpellOwner = spellOwner->_getUObject();
//	m_SpellNiagara = spellData->SpellNiagara;
//
//	if(m_SpellNiagara.IsNull() == true)
//	{
//		LOG_ERROR("[BASE SPELL] Tried to Init spell with invalid SpellNiagara");
//		return;
//	}
//
//	m_Cooldown = spellData->SpellCooldown;
//
//	UCoreFunctionLibrary::RequestAsyncLoad(m_SpellNiagara.ToSoftObjectPath());
//}

//void UBaseSpell::SetDecoratedSelf(ISpell* decoratedSelf)
//{
//	if (decoratedSelf == nullptr)
//	{
//		LOG_ERROR("[BASE SPELL] Tried to set Spell DecoratedSelf with invalid spell");
//		return;
//	}
//
//	m_DecoratedSelf = decoratedSelf->_getUObject();
//}

//void UBaseSpell::LoadHitNiagara()
//{
//
//}
//
//bool UBaseSpell::CanCast()
//{
//	if (m_CooldownTimer > 0.0f)
//	{
//		return false;
//	}
//
//	return true;
//}
//
//bool UBaseSpell::CastSpell()
//{
//	if (m_CooldownTimer > 0.0f)
//	{
//		LOG_WARNING("[BASE SPELL] Check CanCast before calling CastSpell");
//		return false;
//	}
//	
//	m_CooldownTimer = m_Cooldown;
//	return true;
//}
//
//void UBaseSpell::Update(float deltaTime)
//{
//	if (m_Cooldown < 0)
//	{
//		return;
//	}
//
//	m_CooldownTimer -= deltaTime;
//}


//void UBaseSpell::Fire(const FVector& direction)
//{
//}

//ISpellCaster* UBaseSpell::GetSpellOwner()
//{
//	if (m_SpellOwner == nullptr)
//	{
//		LOG_ERROR("[BASE SPELL] Spell has invalid SpellOwner");
//		return nullptr;
//	}
//
//	return m_SpellOwner.GetInterface();
//}

//void UBaseSpell::SpawnSpellNiagara(const FVector& spellLocation)
//{
//	if (m_SpellNiagara.IsValid() == false)
//	{
//		return;
//	}
//
//	if (UWorld* worldContext = UCoreFunctionLibrary::GetGameWorld())
//	{
//		UNiagaraFunctionLibrary::SpawnSystemAtLocation(worldContext, m_SpellNiagara.Get(), spellLocation);
//	}
//}