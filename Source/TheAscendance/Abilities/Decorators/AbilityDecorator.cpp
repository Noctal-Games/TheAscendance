// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityDecorator.h"
#include "TheAscendance/Core/CoreMacros.h"

void UAbilityDecorator::Decorate(IAbility* decorator)
{
	if (decorator == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Tried to decorate Ability with invalid decorator");
		return;
	}

	m_DecoratedAbility = decorator->_getUObject();
}

void UAbilityDecorator::SetDecoratedSelf(IAbility* decoratedSelf)
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return;
	}

	m_DecoratedAbility->SetDecoratedSelf(decoratedSelf);
}

void UAbilityDecorator::Start()
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return;
	}

	m_DecoratedAbility->Start();
}

void UAbilityDecorator::Stop()
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return;
	}

	m_DecoratedAbility->Stop();
}

void UAbilityDecorator::Execute()
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return;
	}

	m_DecoratedAbility->Execute();
}

const FGameplayTag& UAbilityDecorator::GetAbilityTag() const
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return FGameplayTag::EmptyTag;
	}

	return m_DecoratedAbility->GetAbilityTag();
}

bool UAbilityDecorator::CanStart() const
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return false;
	}

	return m_DecoratedAbility->CanStart();
}

void UAbilityDecorator::OnInputReleased()
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return;
	}

	m_DecoratedAbility->OnInputReleased();
}

float UAbilityDecorator::PlayAnimMontageOnOwner(UAnimMontage* animation)
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return false;
	}

	return m_DecoratedAbility->PlayAnimMontageOnOwner(animation);
}
