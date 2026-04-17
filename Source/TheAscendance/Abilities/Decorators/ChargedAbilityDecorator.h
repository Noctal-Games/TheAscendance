// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityDecorator.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Abilities/Structs/AbilityModifierData.h"
#include "ChargedAbilityDecorator.generated.h"

class IAbility;
class UAnimMontage;

UCLASS()
class THEASCENDANCE_API UChargedAbilityDecorator : public UAbilityDecorator
{
	GENERATED_BODY()
	
public:
	class THEASCENDANCE_API Builder
	{
		UPROPERTY()
		TWeakObjectPtr<UChargedAbilityDecorator> m_Decorator = nullptr;

	public:
		Builder(IAbility* decorator, const FChargedAbilityModifier& modifierData)
		{
			m_Decorator = NewObject<UChargedAbilityDecorator>();
			m_Decorator->Decorate(decorator);

			m_Decorator->m_ModifierData = MakeShared<FChargedAbilityModifier>(modifierData);
		}
		IAbility* Build()
		{
			if (m_Decorator.IsValid() == false)
			{
				LOG_ERROR("[CHARGED ABILITY DECORATOR] Tried to build without initialising it.")
				return nullptr;
			}

			m_Decorator->LoadAnimation();
			return m_Decorator.Get();
		}
	};

	virtual void Start() override;
	virtual void Stop() override;

	virtual void OnInputReleased() override;

protected:
	void LoadAnimation();
	void SetToCharged();

private:
	TSharedPtr<FChargedAbilityModifier> m_ModifierData = nullptr;

	UPROPERTY()
	TSoftObjectPtr<UAnimMontage> m_ChargingAnimation = nullptr;

	FTimerHandle m_ChargeDurationHandle;

	bool m_IsCharging = false;
	bool m_IsCharged = false;
};

/*void UChargedAbilityDecorator::Start()
{
	if (m_ChargingAnimation.IsValid() == false)
	{
		LOG_ERROR("[CHARGED ABILITY DECORATOR] Tried to Start without valid ChargingAnimation");
		return;
	}

	//If animation is looped, just use the max charge time. If not, use the animation duration as the charge time.
	float animationDuration = m_DecoratedAbility->PlayAnimMontageOnOwner(m_ChargingAnimation.Get());
	m_ChargeDuration = m_ModifierData->IsAnimationLooped ? m_ModifierData->MaxChargeTime : animationDuration;

	if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		m_ChargeStartTime = world->GetTimeSeconds();
	}
}

void UChargedAbilityDecorator::OnInputReleased()
{
	m_DecoratedAbility->OnInputReleased();

	float time = 0.0f;

	if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		time = world->GetTimeSeconds();
	}

	float elapsed = time - m_ChargeStartTime;
	bool m_IsCharged = elapsed >= m_ChargeDuration;

	if (m_IsCharged == false)
	{
		LOG_ONSCREEN(-1, 5.0f, FColor::Yellow, "Ability - %s: CANCELLED", *GetAbilityTag().ToString());
		return;
	}

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

	UCoreFunctionLibrary::RequestAsyncLoad(m_ChargingAnimation.ToSoftObjectPath());
}*/