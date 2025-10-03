// Fill out your copyright notice in the Description page of Project Settings.


#include "AOEDeliveryEffect.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/GameplayTagHelpers.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Effects/Structs/EffectData.h"
#include "TheAscendance/Characters/Interfaces/Susceptible.h"

#include "TimerManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

bool UAOEDeliveryEffect::Init(UEffectData* effectData)
{
	if (UAreaOfEffectDeliveryEffectData* data = Cast<UAreaOfEffectDeliveryEffectData>(effectData))
	{
		m_EffectData = data;

		FGameplayTag baseEffectTag = UGameplayTagHelpers::GetBaseTag(m_EffectData->EffectTag);

		UEffectData* effectToApplyData = nullptr;

		if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
		{
			effectToApplyData = gameMode->LoadEffectData(baseEffectTag);
		}

		if (effectToApplyData == nullptr)
		{
			LOG_ERROR("Tried to Init AOEDeliveryEffectData but could not find EffectData for it's BaseEffect");
			return false;
		}

		if (UCoreEffectData* effectToApply = Cast<UCoreEffectData>(effectToApplyData))
		{
			m_EffectToApply = effectToApply;
		}
		else
		{
			LOG_ERROR("Tried to Init AOEDeliveryEffect with invalid EffectData for the BaseEffect. DeliveryEffects can only apply CoreEffects");
			return false;
		}

		return true;
	}
	else
	{
		LOG_ERROR("Tried to Init AOEDeliveryEffect with invalid EffectData");
		return false;
	}
}

void UAOEDeliveryEffect::StartEffect(ISusceptible* target, FVector location)
{
	AddToRoot();
	LOG_ONSCREEN(-1, 5.0f, FColor::Red, "AOEDeliveryEffect is being added to root");

	m_HasEnded = false;

	if (m_EffectData == nullptr || (m_EffectToApply == nullptr && m_EffectData->ApplyPhysics == false))
	{
		EndEffect();
		return;
	}

	m_Location = location;

	m_Timer = m_EffectData->Duration;
	m_Interval = m_EffectData->EffectInterval;

	DoEffect();

	if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		world->GetTimerManager().SetTimer(m_TimerHandle, this, &UAOEDeliveryEffect::DoEffect, m_Interval, true);
	}
}

UEffectData* UAOEDeliveryEffect::GetEffectData()
{
	if (m_EffectData.IsValid() == false)
	{
		LOG_ERROR("AOEDeliveryEffect has no valid EffectData");
		return nullptr;
	}

	return m_EffectData.Get();
}

void UAOEDeliveryEffect::EndEffect()
{
	if (m_HasEnded == true)
	{
		return;
	}

	m_HasEnded = true;

	if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		world->GetTimerManager().ClearTimer(m_TimerHandle);
	}

	RemoveFromRoot();

	//Debug
	//CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS); // Forces immediate GC to check if everything is working.
	LOG_ONSCREEN(-1, 5.0f, FColor::Red, "AOEDeliveryEffect is being removed from root");
}

void UAOEDeliveryEffect::BeginDestroy()
{
	Super::BeginDestroy();

	//Debug
	LOG_ONSCREEN(-1, 5.0f, FColor::Red, "AOEDeliveryEffect is being destroyed");
}

void UAOEDeliveryEffect::DoEffect()
{
	if (m_EffectData.IsValid() == false)
	{
		LOG_ERROR("AOEDeliveryEffect has no valid EffectData");
		EndEffect();
		return;
	}

	if (m_HasEnded == true)
	{
		return;
	}

	m_Timer -= m_Interval;

	TArray<AActor*> actors;
	ScanForTargets(m_Location, m_EffectData->DeliveryRange, actors);

	for (AActor* actor : actors)
	{
		if (ISusceptible* target = Cast<ISusceptible>(actor))
		{
			ApplyEffect(target);
		}

		if (m_EffectData->ApplyPhysics == true)
		{
			FVector direction = actor->GetActorLocation() - m_Location;

			if (m_EffectData->PushOrPull == false)
			{
				direction = -direction;
			}

			direction.Normalize();

			float distance = FVector::Distance(actor->GetActorLocation(), m_Location);
			float normalizedDistance = distance / m_EffectData->DeliveryRange;

			float strength = m_EffectData->Strength - (m_EffectData->Strength - 0.0f) * normalizedDistance * normalizedDistance;
			strength = FMath::Clamp(strength, 0.0f, m_EffectData->Strength);

			if (ACharacter* character = Cast<ACharacter>(actor))
			{
				float deltaSeconds = 0.0f;

				if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
				{
					deltaSeconds = world->GetDeltaSeconds();
				}

				if (UCharacterMovementComponent* moveComp = character->GetCharacterMovement())
				{
					FVector force = direction * strength;
					moveComp->Velocity += force;
				}

				//character->LaunchCharacter(direction * strength, true, true);
				continue;
			}

			if (UPrimitiveComponent* primitiveComponent = actor->FindComponentByClass<UPrimitiveComponent>())
			{
				if (primitiveComponent->IsSimulatingPhysics() == false)
				{
					continue;
				}

				primitiveComponent->AddImpulse(direction * strength, NAME_None, true);
			}
		}
	}

	if (m_Timer <= 0.0f)
	{
		EndEffect();
		return;
	}
}
