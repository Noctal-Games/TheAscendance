// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEffect.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Characters/Interfaces/Susceptible.h"

bool UBaseEffect::Init(UEffectData* effectData)
{
	return false;
}

void UBaseEffect::StartEffect(ISusceptible* target, FVector location)
{
	m_HasEnded = false;

	if (target == nullptr)
	{
		LOG_ERROR("Tried to Start Effect with invalid Target");

		if (OnEffectEnd.IsBound())
		{
			OnEffectEnd.Execute(this);
		}

		return;
	}

	m_Target = target->_getUObject();
}

void UBaseEffect::EndEffect()
{
	m_HasEnded = true;

	if (OnEffectEnd.IsBound())
	{
		OnEffectEnd.Execute(this);
	}
}

void UBaseEffect::ResetEffect()
{
}

UEffectData* UBaseEffect::GetEffectData()
{
	return nullptr;
}

void UBaseEffect::DoEffect()
{

}