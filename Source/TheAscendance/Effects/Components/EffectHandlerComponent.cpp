// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectHandlerComponent.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Effects/Structs/EffectData.h"
#include "TheAscendance/Effects/BaseEffect.h"
#include "TheAscendance/Characters/Interfaces/Susceptible.h"

// Sets default values for this component's properties
UEffectHandlerComponent::UEffectHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UEffectHandlerComponent::Init(ISusceptible* owner)
{
	if (owner == nullptr)
	{
		LOG_ERROR("Tried to Init EffectHandlerComponent with invalid owner");
		return;
	}

	m_Owner = owner->_getUObject();
}

void UEffectHandlerComponent::AddEffect(UBaseEffect* effect)
{
	if (effect == nullptr || m_Owner == nullptr)
	{
		LOG_ERROR("Tried to add Effect, but the Effect or Owner was invalid");
		return;
	}

	UCoreEffectData* effectData = nullptr;
	
	if (UEffectData* baseData = effect->GetEffectData())
	{
		effectData = Cast<UCoreEffectData>(baseData);
	}

	if (effectData == nullptr)
	{
		LOG_ERROR("Tried to add Effect, but failed to get EffectData");
		return;
	}

	FGameplayTag* tag = &effectData->EffectTag;

	if (m_Effects.Contains(*tag) == false)
	{
		m_Effects.Add(effectData->EffectTag, FEffectList());
		//Add Niagara
	}

	if (effectData->CanStack == true)
	{
		if (m_Effects[*tag].Effects.Num() == effectData->StackCap)
		{
			if (UBaseEffect* oldEffect = m_Effects[*tag].Effects[0])
			{
				oldEffect->EndEffect();
			}
		}
	}
	else
	{
		if (m_Effects[*tag].Effects.Num() != 0)
		{
			if (UBaseEffect* oldEffect = m_Effects[*tag].Effects[0])
			{
				oldEffect->EndEffect();
			}
		}
	}

	m_Effects[*tag].Effects.Add(effect);
	effect->OnEffectEnd.BindLambda([this](UBaseEffect* toRemove) { QueueRemoval(toRemove); });
	effect->StartEffect(m_Owner.GetInterface());
}

void UEffectHandlerComponent::QueueRemoval(UBaseEffect* effect)
{
	if (effect == nullptr)
	{
		LOG_ERROR("Tried to QueueRemoval for invalid Effect");
		return;
	}

	if (UEffectData* data = effect->GetEffectData())
	{
		if (m_Effects.Contains(data->EffectTag) == false || m_Effects[data->EffectTag].Effects.Contains(effect) == false)
		{
			LOG_ERROR("Tried to QueueRemoval for Effect that isn't stored by the EffectComponent");
			return;
		}

		if (m_EffectsToRemove.Contains(data->EffectTag) == false)
		{
			m_EffectsToRemove.Add(data->EffectTag, FEffectList());
		}

		if (m_EffectsToRemove[data->EffectTag].Effects.Contains(effect) == false)
		{
			m_EffectsToRemove[data->EffectTag].Effects.Add(effect);
		}
	}
}

void UEffectHandlerComponent::RemoveEffects()
{
	TArray<FGameplayTag> tags;
	m_EffectsToRemove.GetKeys(tags);

	TArray<FGameplayTag> tagsToClear;

	for (const FGameplayTag& effectTag : tags)
	{
		for (UBaseEffect* effect : m_EffectsToRemove[effectTag].Effects)
		{
			m_Effects[effectTag].Effects.Remove(effect);
		}

		if (m_Effects[effectTag].Effects.IsEmpty() == true)
		{
			tagsToClear.Add(effectTag);
		}
	}

	for (const FGameplayTag& effectTag : tagsToClear)
	{
		if (m_Effects.Contains(effectTag) == false)
		{
			continue;
		}

		m_Effects.Remove(effectTag);
	}

	m_EffectsToRemove.Empty();
}

void UEffectHandlerComponent::ClearEffects()
{
	TArray<FGameplayTag> tags;
	m_Effects.GetKeys(tags);

	for (const FGameplayTag& effectTag : tags)
	{
		for (UBaseEffect* effect : m_Effects[effectTag].Effects)
		{
			effect->EndEffect();
		}
	}

	m_Effects.Empty();
}


// Called when the game starts
void UEffectHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEffectHandlerComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	if (m_Effects.IsEmpty() == true)
	{
		return;
	}

	TArray<FGameplayTag> tags;
	m_Effects.GetKeys(tags);

	for (const FGameplayTag& effectTag : tags)
	{
		if (m_Effects[effectTag].Effects.Num() == 0)
		{
			continue;
		}

		for (UBaseEffect* effect : m_Effects[effectTag].Effects)
		{
			if (effect == nullptr)
			{
				continue;
			}

			effect->Update(deltaTime);
		}
	}

	if (m_EffectsToRemove.IsEmpty() == false)
	{
		RemoveEffects();
	}
}

