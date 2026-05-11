// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectHandlerComponent.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/StreamableFunctionLibrary.h"
#include "TheAscendance/Effects/Structs/EffectData.h"
#include "TheAscendance/Effects/CoreEffect.h"
#include "TheAscendance/Characters/Interfaces/Susceptible.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

// Sets default values for this component's properties
UEffectHandlerComponent::UEffectHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

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

void UEffectHandlerComponent::AddEffect(UCoreEffect* effect)
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

	if (m_Owner->HasImmunity(*tag) == true)
	{
		return;
	}

	if (m_Effects.Contains(*tag) == false)
	{
		m_Effects.Add(effectData->EffectTag, FEffectList());

		if (effectData->EffectNiagara.IsNull() == false)
		{
			m_NiagaraSystems.Add(effectData->EffectTag, effectData->EffectNiagara);
			m_NiagaraComponents.Add(effectData->EffectTag, nullptr);

			UStreamableFunctionLibrary::RequestAsyncLoad(m_NiagaraSystems[effectData->EffectTag].ToSoftObjectPath(), [this, effectData]()
				{

					if (m_NiagaraSystems[effectData->EffectTag].IsValid())
					{
						m_NiagaraComponents[effectData->EffectTag] = UNiagaraFunctionLibrary::SpawnSystemAttached(
							m_NiagaraSystems[effectData->EffectTag].Get(),
							m_Owner->GetSusceptibleActor()->GetRootComponent(),
							"",
							m_Owner->GetSusceptibleActor()->GetActorLocation(),
							FRotator::ZeroRotator,
							EAttachLocation::KeepWorldPosition, true, true, ENCPoolMethod::AutoRelease, true);

						if (m_NiagaraComponents[effectData->EffectTag] != nullptr)
						{
							m_NiagaraComponents[effectData->EffectTag]->Activate();
						}
					}
				});
		}

		//Add Niagara
	}

	if (effectData->CanStack == true)
	{
		if (m_Effects[*tag].Effects.Num() == effectData->StackCap)
		{
			if (UCoreEffect* oldEffect = m_Effects[*tag].Effects[0])
			{
				oldEffect->ResetEffect();

				if (m_Effects[*tag].Effects.Num() > 1)
				{
					m_Effects[*tag].Effects.RemoveAt(0);
					m_Effects[*tag].Effects.Add(oldEffect);
				}

				return;
			}
		}
	}
	else
	{
		if (m_Effects[*tag].Effects.Num() != 0)
		{
			if (UCoreEffect* oldEffect = m_Effects[*tag].Effects[0])
			{
				oldEffect->ResetEffect();
				return;
			}
		}
	}

	m_Effects[*tag].Effects.Add(effect);
	effect->OnEffectEnd.BindLambda([this](UBaseEffect* toRemove) {     

		if (UCoreEffect* Core = Cast<UCoreEffect>(toRemove))
		{
			RemoveEffect(Core);
		};
	});

	effect->StartEffect(m_Owner.GetInterface());
}

void UEffectHandlerComponent::RemoveEffect(UCoreEffect* effect)
{
	if (effect == nullptr)
	{
		LOG_ERROR("Tried to Remove invalid Effect");
		return;
	}

	if (UEffectData* data = effect->GetEffectData())
	{
		if (m_Effects.Contains(data->EffectTag) == false || m_Effects[data->EffectTag].Effects.Contains(effect) == false)
		{
			LOG_ERROR("Tried to Remove Effect that isn't stored by the EffectComponent");
			return;
		}

		m_Effects[data->EffectTag].Effects.Remove(effect);

		if (m_Effects[data->EffectTag].Effects.IsEmpty())
		{
			m_Effects.Remove(data->EffectTag);

			if (m_NiagaraSystems.Contains(data->EffectTag))
			{
				m_NiagaraSystems.Remove(data->EffectTag);
			}

			if (m_NiagaraComponents.Contains(data->EffectTag))
			{
				if (m_NiagaraComponents[data->EffectTag] != nullptr)
				{
					m_NiagaraComponents[data->EffectTag]->Deactivate();
				}

				m_NiagaraComponents.Remove(data->EffectTag);
			}
		}
	}
}

void UEffectHandlerComponent::ClearEffects()
{
	TArray<FGameplayTag> tags;
	m_Effects.GetKeys(tags);

	for (const FGameplayTag& effectTag : tags)
	{
		for (UCoreEffect* effect : m_Effects[effectTag].Effects)
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

