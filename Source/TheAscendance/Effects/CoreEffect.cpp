// Fill out your copyright notice in the Description page of Project Settings.


#include "CoreEffect.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Characters/Interfaces/Susceptible.h"

void UCoreEffect::StartEffect(ISusceptible* target)
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

void UCoreEffect::EndEffect()
{
	m_HasEnded = true;

	if (OnEffectEnd.IsBound())
	{
		OnEffectEnd.Execute(this);
	}
}