// Fill out your copyright notice in the Description page of Project Settings.


#include "ChainDeliveryEffect.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/GameplayTagHelpers.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Effects/Structs/EffectData.h"
#include "TheAscendance/Effects/CoreEffect.h"

#include "TimerManager.h"

bool UChainDeliveryEffect::Init(UEffectData* effectData)
{
	if (UChainDeliveryEffectData* data = Cast<UChainDeliveryEffectData>(effectData))
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
			LOG_ERROR("Tried to Init ChainDeliveryEffectData but could not find EffectData for it's BaseEffect");
			return false;
		}

		if (UCoreEffectData* effectToApply = Cast<UCoreEffectData>(effectToApplyData))
		{
			m_EffectToApply = effectToApply;
		}
		else
		{
			LOG_ERROR("Tried to Init ChainDeliveryEffectData with invalid EffectData for the BaseEffect. DeliveryEffects can only apply CoreEffects");
			return false;
		}

		return true;
	}
	else
	{
		LOG_ERROR("Tried to Init ChainDeliveryEffect with invalid EffectData");
		return false;
	}
}

void UChainDeliveryEffect::StartEffect(ISusceptible* target, FVector location)
{
	m_HasEnded = false;

	if (m_EffectData == nullptr || m_EffectToApply == nullptr || target == nullptr)
	{
		UnRoot();
		return;
	}

	m_BounceQueue.Empty();
	m_AffectedTargets.Empty();

	m_BounceQueue.Enqueue(FChainBounce(target, m_EffectData->DeliveryRange, 0));

	ProcessNextBounce();
}

UEffectData* UChainDeliveryEffect::GetEffectData()
{
	if (m_EffectData.IsValid() == false)
	{
		LOG_ERROR("ChainDeliveryEffect has no valid EffectData");
		return nullptr;
	}

	return m_EffectData.Get();
}
void UChainDeliveryEffect::Root()
{
	AddToRoot();

	//Debug
	LOG_ONSCREEN(-1, 5.0f, FColor::Red, "ChainDeliveryEffect is being added to root");
}

void UChainDeliveryEffect::UnRoot()
{
	if (m_HasEnded == true)
	{
		return;
	}

	m_HasEnded = true;

	if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		world->GetTimerManager().ClearTimer(m_BounceTimerHandle);
		world->GetTimerManager().ClearAllTimersForObject(this);
	}

	m_BounceQueue.Empty();
	m_AffectedTargets.Empty();

	RemoveFromRoot();
	
	//Debug
	//CollectGarbage(GARBAGE_COLLECTION_KEEPFLAGS); // Forces immediate GC to check if everything is working.
	LOG_ONSCREEN(-1, 5.0f, FColor::Red, "ChainDeliveryEffect is being removed from root");
}

void UChainDeliveryEffect::BeginDestroy()
{
	Super::BeginDestroy();

	//Debug
	LOG_ONSCREEN(-1, 5.0f, FColor::Red, "ChainDeliveryEffect is being destroyed");
}

void UChainDeliveryEffect::ProcessNextBounce()
{
	if (m_HasEnded == true)
	{
		return;
	}

	FChainBounce currentBounce;

	if (m_BounceQueue.Dequeue(currentBounce) == false)
	{
		LOG_ERROR("Chain BounceQueue failed to Dequeue");
		UnRoot();
		return;
	}

	if (currentBounce.Source == nullptr)
	{
		return;
	}

	ISusceptible* current = currentBounce.Source.GetInterface();

	if (current == nullptr || m_AffectedTargets.Contains(current->_getUObject()))
	{
		if (m_BounceQueue.IsEmpty() == false)
		{
			if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
			{
				world->GetTimerManager().SetTimerForNextTick(this, &UChainDeliveryEffect::ProcessNextBounce);
			}
		}

		return;
	}

	m_AffectedTargets.Add(current->_getUObject());
	ApplyEffect(current);

	TArray<AActor*> actors;
	ScanForTargets(Cast<AActor>(current->_getUObject())->GetActorLocation(), currentBounce.Radius, actors);

	for (AActor* actor : actors)
	{
		if (ISusceptible* target = Cast<ISusceptible>(actor))
		{
			if (m_AffectedTargets.Contains(target->_getUObject()) == false)
			{
				m_BounceQueue.Enqueue(FChainBounce(target, currentBounce.Radius, currentBounce.BounceIndex + 1));
			}
		}
	}

	if (m_BounceQueue.IsEmpty() == false)
	{
		if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
		{
			world->GetTimerManager().SetTimer(m_BounceTimerHandle, this, &UChainDeliveryEffect::ProcessNextBounce, m_EffectData == nullptr ? 0.5f : m_EffectData->BounceDelay, false);
		}
	}
	else
	{
		UnRoot();
	}
}
