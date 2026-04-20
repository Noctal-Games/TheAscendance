// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAbility.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "Structs/AbilityData.h"
#include "Components/AbilityComponent.h"

void UBaseAbility::Init(UAbilityComponent* ownerComponent, UAbilityData* abilityData)
{
	m_OwnerComponent = ownerComponent;

	if(m_OwnerComponent == nullptr)
	{
		LOG_ERROR("[BASE ABILITY] Failed to Init Ability, OwnerComponent was invalid");
		return;
	}

	m_AbilityData = abilityData;

	if (m_AbilityData == nullptr)
	{
		LOG_ERROR("[BASE ABILITY] Failed to Init Ability, AbilityData was invalid");
		return;
	}

	m_AbilityAnimation = m_AbilityData->AbilityAnimation;

	if(m_AbilityAnimation.IsNull() == true)
	{
		LOG_ERROR("[BASE ABILITY] Ability - %s: AbilityAnimation was invalid", *GetAbilityTag().ToString());
		return;
	}

	UCoreFunctionLibrary::RequestAsyncLoad(m_AbilityAnimation.ToSoftObjectPath());
}

void UBaseAbility::SetDecoratedSelf(IAbility* decoratedSelf)
{	
	if (decoratedSelf == nullptr)
	{
		LOG_ERROR("[BASE ABILITY] Tried to set Ability DecoratedSelf with invalid ability");
		return;
	}

	m_DecoratedSelf = decoratedSelf->_getUObject();
}

void UBaseAbility::Start()
{
	LOG_ONSCREEN(-1, 5.0f, FColor::Yellow, "Ability - %s: STARTED", *GetAbilityTag().ToString());
	Execute();
}

void UBaseAbility::Stop()
{
	LOG_ONSCREEN(-1, 5.0f, FColor::Yellow, "Ability - %s: STOP", *GetAbilityTag().ToString());

	if (m_AbilityDurationHandle.IsValid())
	{
		UCoreFunctionLibrary::ClearTimerHandle(m_AbilityDurationHandle, FString("Stop Ability"));
	}
}

void UBaseAbility::Execute()
{
	LOG_ONSCREEN(-1, 5.0f, FColor::Yellow, "Ability - %s: EXECUTE", *GetAbilityTag().ToString());

	if (m_AbilityAnimation.IsNull() == true)
	{
		LOG_ERROR("[BASE ABILITY] Start was called but AbilityAnimation is invalid");
		return;
	}

	//Timer to prevent animation locking up attacks. If an animation fails to notify, the ability and character attack state will be reset by default. 
	float duration = PlayAnimMontageOnOwner(m_AbilityAnimation.Get());
	UCoreFunctionLibrary::SetTimer(m_AbilityDurationHandle, this, &UBaseAbility::Stop, duration);
}

void UBaseAbility::TriggerAbility()
{
	LOG_ONSCREEN(-1, 5.0f, FColor::Yellow, "Ability - %s: TRIGGER", *GetAbilityTag().ToString());
}

void UBaseAbility::OnInputReleased()
{
	LOG_ONSCREEN(-1, 5.0f, FColor::Yellow, "Ability - %s: RELEASED", *GetAbilityTag().ToString());
}

const FGameplayTag& UBaseAbility::GetAbilityTag() const
{
	if (m_AbilityData == nullptr)
	{
		LOG_ERROR("[BASE ABILITY] GetAbilityTag was called but AbilityData is invalid");
		return FGameplayTag::EmptyTag;
	}

	return m_AbilityData->AbilityTag;
}

float UBaseAbility::PlayAnimMontageOnOwner(UAnimMontage* animation)
{
	if (m_OwnerComponent.IsValid() == false)
	{
		LOG_ERROR("[BASE ABILITY] PlayAnimMontageOnOwner was called but OwnerComponent is invalid");
		return 0.0f;
	}

	return m_OwnerComponent->PlayAnimMontageOnOwner(animation);
}

UAbilityData* UBaseAbility::GetAbilityData()
{
	return m_AbilityData;
}

AActor* UBaseAbility::GetAbilityOwner()
{
	if (m_OwnerComponent.IsValid() == false)
	{
		LOG_ERROR("[BASE ABILITY] GetOwner was called but OwnerComponent is invalid");
		return nullptr;
	}

	return m_OwnerComponent->GetOwner();
}
