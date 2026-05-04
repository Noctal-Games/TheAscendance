// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityDecorator.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Abilities/Structs/AbilityInfo.h"

void UAbilityDecorator::Decorate(IAbility* decorator)
{
	if (decorator == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Tried to decorate Ability with invalid decorator");
		return;
	}

	m_DecoratedAbility = decorator->_getUObject();
}

void UAbilityDecorator::Init(UAbilityComponent* ownerComponent, UAbilityData* abilityData)
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return;
	}

	m_DecoratedAbility->Init(ownerComponent, abilityData);
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

void UAbilityDecorator::TriggerAbility()
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return;
	}

	m_DecoratedAbility->TriggerAbility();
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

void UAbilityDecorator::Update(float deltaTime)
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return;
	}

	m_DecoratedAbility->Update(deltaTime);
}

void UAbilityDecorator::OnOverlap(AActor* overlapActor, const FVector& spellOverlapLocation, int damage)
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return;
	}

	m_DecoratedAbility->OnOverlap(overlapActor, spellOverlapLocation, damage);
}

void UAbilityDecorator::OnHit(AActor* hitActor, const FVector& spellHitLocation)
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return;
	}

	m_DecoratedAbility->OnHit(hitActor, spellHitLocation);
}

void UAbilityDecorator::ProcessHit(const FVector& spellHitLocation)
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return;
	}

	m_DecoratedAbility->ProcessHit(spellHitLocation);
}

void UAbilityDecorator::SpawnHitNiagara(const FVector& spellHitLocation)
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return;
	}

	return m_DecoratedAbility->SpawnHitNiagara(spellHitLocation);
}

void UAbilityDecorator::LoadHitNiagara()
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return;
	}

	return m_DecoratedAbility->LoadHitNiagara();
}

TArray<TObjectPtr<AActor>> UAbilityDecorator::GetHitActors()
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return TArray<TObjectPtr<AActor>>();
	}

	return m_DecoratedAbility->GetHitActors();
}

void UAbilityDecorator::ProcessOverlapDamage(int& damage)
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return;
	}

	m_DecoratedAbility->ProcessOverlapDamage(damage);
}

void UAbilityDecorator::ProcessHitDamage(int& damage, const FVector& targetLocation, const FVector& hitLocation)
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return;
	}

	m_DecoratedAbility->ProcessHitDamage(damage, targetLocation, hitLocation);
}

bool UAbilityDecorator::DealDamage(AActor* hitActor, int damage)
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return false;
	}

	return m_DecoratedAbility->DealDamage(hitActor, damage);
}

void UAbilityDecorator::ApplyEffects(AActor* hitActor)
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return ;
	}

	m_DecoratedAbility->ApplyEffects(hitActor);
}

UAbilityData* UAbilityDecorator::GetAbilityData()
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return nullptr;
	}

	return m_DecoratedAbility->GetAbilityData();
}

AActor* UAbilityDecorator::GetAbilityOwner()
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return nullptr;
	}

	return m_DecoratedAbility->GetAbilityOwner();
}

const FAbilityInfo& UAbilityDecorator::GetAbilityInfo() const
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		static const FAbilityInfo emptyInfo;
		return emptyInfo;
	}

	return m_DecoratedAbility->GetAbilityInfo();
}

void UAbilityDecorator::AddSlot(EAbilitySlot slot)
{
	if (m_DecoratedAbility == nullptr)
	{
		LOG_ERROR("[ABILITY DECORATOR] Invalid DecoratedAbility");
		return;
	}

	m_DecoratedAbility->AddSlot(slot);
}
