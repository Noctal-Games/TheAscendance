// Fill out your copyright notice in the Description page of Project Settings.


#include "ChargedAbilityDecorator.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/StreamableFunctionLibrary.h"
#include "TheAscendance/Abilities/Structs/AbilityInfo.h"
#include "TheAscendance/Abilities/Enums/AbilitySlot.h"

void UChargedAbilityDecorator::Start()
{
	const FAbilityInfo& info = m_DecoratedAbility->GetAbilityInfo();

	if (info.SlotsUsed.Contains(EAbilitySlot::MAINHAND_PRIMARY) || info.SlotsUsed.Contains(EAbilitySlot::OFFHAND_PRIMARY))
	{
		LOG_ERROR("[CHARGED ABILITY DECORATOR] Ability - %s: Is charged and using a primary slot, the ability is unchargeable due to input setup.", *GetAbilityTag().ToString());
		return;
	}

	m_IsCharged = false;

	if (m_ChargingAnimation.IsValid() == false)
	{
		LOG_ERROR("[CHARGED ABILITY DECORATOR] Tried to Start without valid ChargingAnimation");
		return;
	}

	m_IsCharging = true;

	//If animation is looped, just use the max charge time. If not, use the animation duration as the charge time.
	float animationDuration = m_DecoratedAbility->PlayAnimMontageOnOwner(m_ChargingAnimation.Get());
	float duration = m_ModifierData->IsAnimationLooped ? m_ModifierData->MaxChargeTime : animationDuration;
	LOG_ONSCREEN(-1, 5.0f, FColor::Yellow, "%f", duration);

	UCoreFunctionLibrary::SetTimer(m_ChargeDurationHandle, this, &UChargedAbilityDecorator::SetToCharged, duration);
}

void UChargedAbilityDecorator::Stop()
{
	m_DecoratedAbility->Stop();

	if (m_ChargeDurationHandle.IsValid())
	{
		UCoreFunctionLibrary::ClearTimerHandle(m_ChargeDurationHandle, FString("Stop Charge"));
	}
}

void UChargedAbilityDecorator::OnInputReleased()
{
	if (m_IsCharging == false)
	{
		return;
	}

	m_IsCharging = false;
	m_DecoratedAbility->OnInputReleased();

	if (m_IsCharged == false)
	{
		LOG_ONSCREEN(-1, 5.0f, FColor::Yellow, "Ability - %s: CANCELLED", *GetAbilityTag().ToString());
		return;
	}

	m_IsCharged = false;

	LOG_ONSCREEN(-1, 5.0f, FColor::Yellow, "Ability - %s: CHARGED", *GetAbilityTag().ToString());
	m_DecoratedAbility->Execute();
}

void UChargedAbilityDecorator::LoadAnimation()
{
	if (m_ModifierData == nullptr)
	{
		return;
	}

	m_ChargingAnimation = m_ModifierData->ChargingAnimation;

	if(m_ChargingAnimation.IsNull() == true)
	{
		LOG_ERROR("[CHARGED ABILITY DECORATOR] Tried to Init with invalid ChargingAnimation");
		return;
	}

	UStreamableFunctionLibrary::RequestAsyncLoad(m_ChargingAnimation.ToSoftObjectPath());
}

void UChargedAbilityDecorator::SetToCharged()
{
	LOG_ONSCREEN(-1, 5.0f, FColor::Yellow, "Ability - %s: CHARGED TRUE", *GetAbilityTag().ToString());

	m_IsCharged = true;

	if (m_ModifierData->DoesChargeCompleteForceAbility == true)
	{
		OnInputReleased();
	}

	if (m_ChargeDurationHandle.IsValid())
	{
		UCoreFunctionLibrary::ClearTimerHandle(m_ChargeDurationHandle, FString("Stop Charge"));
	}
}