// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatManagerSubsystem.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/AI/Combat/Components/CombatAIComponent.h"

void UCombatManagerSubsystem::RegisterAIComponent(UCombatAIComponent* component)
{
	if (component == nullptr)
	{
		LOG_ERROR("[COMBAT MANAGER] Tried to Register invalid component");
		return;
	}

	if (m_RegisteredComponents.Contains(component) == false)
	{
		m_RegisteredComponents.Add(component);
	}
}

void UCombatManagerSubsystem::UnRegisterAIComponent(UCombatAIComponent* component)
{
	if (component == nullptr)
	{
		LOG_ERROR("[COMBAT MANAGER] Tried to UnRegister invalid component");
		return;
	}

	if (m_RegisteredComponents.Contains(component) == true)
	{
		m_RegisteredComponents.Remove(component);
	}
}

void UCombatManagerSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);
	UCoreFunctionLibrary::SetTimer(m_CleanUpTimer, this, &UCombatManagerSubsystem::CleanRegistry, 10.0f, true);
}

void UCombatManagerSubsystem::Deinitialize()
{
	m_RegisteredComponents.Empty();
	UCoreFunctionLibrary::ClearTimerHandle(m_CleanUpTimer, "COMBAT MANAGER REGISTRY CLEANUP");
	Super::Deinitialize();
}

void UCombatManagerSubsystem::CleanRegistry()
{
	m_RegisteredComponents.RemoveAll([](const TWeakObjectPtr<UCombatAIComponent>& ptr)
		{
			return !ptr.IsValid();
		});
}
