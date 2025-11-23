// Fill out your copyright notice in the Description page of Project Settings.


#include "HSMAgentComponent.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/AI/States/IdleState.h"
#include "TheAscendance/AI/States/InvestigateState.h"
#include "TheAscendance/AI/States/CombatState.h"
#include "TheAscendance/AI/Components/SightSensorComponent.h"
#include "TheAscendance/Characters/Enemies/BaseEnemy.h"
#include "TheAscendance/Characters/Player/PlayerCharacter.h"

// Sets default values for this component's properties
UHSMAgentComponent::UHSMAgentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UHSMAgentComponent::Init(ABaseEnemy* owner)
{
	if (owner == nullptr)
	{
		LOG_ERROR("Tried to Init HSMAgentComponent with invalid owner");
		return;
	}

	m_Owner = owner;
	m_Player = UCoreFunctionLibrary::GetPlayerCharacter();

	if(m_Player.IsValid() == false)
	{
		LOG_ERROR("Tried to Init HSMAgentComponent with invalid player reference");
		return;
	}

	m_CurrentState = EState::MAX;
	m_States.Add(EState::IDLE, NewObject<UIdleState>());
	m_States.Add(EState::INVESTIGATE, NewObject<UInvestigateState>());
	m_States.Add(EState::COMBAT, NewObject<UCombatState>());

	m_SightSensor = NewObject<USightSensorComponent>(m_Owner.Get(), TEXT("SIGHT SENSOR"));
	m_SightSensor->RegisterComponent();
	m_SightSensor->Init(this);
	m_SightSensor->SetTarget(m_Player.Get());
	m_SightSensor->SetIsActive(true);

	if (m_States.Num() != (int32)EState::MAX)
	{
		LOG_ERROR("States count does not match EState length");
	}

	SetState(EState::IDLE);
}

void UHSMAgentComponent::SetState(EState newState)
{
	if (m_CurrentState == newState)
	{
		return;
	}
	else if (m_States.Contains(newState) == false || m_States[newState] == nullptr)
	{
		LOG_ERROR("Tried to set state to an invalid state");
		return;
	}

	if(m_States.Contains(m_CurrentState) && m_States[m_CurrentState] != nullptr)
	{
		m_States[m_CurrentState]->EndState();
	}

	m_CurrentState = newState;
	m_States[m_CurrentState]->StartState(this);
}

void UHSMAgentComponent::SetCombatState(ECombatState newState)
{
	if (m_States.Contains(EState::COMBAT) == false || m_States[EState::COMBAT] == nullptr)
	{
		return;
	}

	if (UCombatState* cState = Cast<UCombatState>(m_States[EState::COMBAT]))
	{
		cState->SetCombatState(newState);
	}
}

void UHSMAgentComponent::SetDestination(const FVector& destination)
{
	if(m_Owner.IsValid() == false)
	{
		LOG_ERROR("Tried to set destination with invalid owner");
		return;
	}

	m_Owner->SetDestination(destination);
}

void UHSMAgentComponent::SetLocationToInvestigate(const FVector& location)
{
	m_LocationToInvestigate = location;

	if (m_CurrentState == EState::IDLE && m_LocationToInvestigate != FVector::ZeroVector)
	{
		SetState(EState::INVESTIGATE);
	}
}

FVector UHSMAgentComponent::GetLocationToInvestigate()
{
	return m_LocationToInvestigate;
}

ABaseEnemy* UHSMAgentComponent::GetAgentOwner() const
{
	if(m_Owner.IsValid() == false)
	{
		LOG_ERROR("Tried to get owner enemy with invalid owner");
		return nullptr;
	}

	return m_Owner.Get();
}

APlayerCharacter* UHSMAgentComponent::GetTargetPlayer() const
{
	if (m_Player.IsValid() == false)
	{
		LOG_ERROR("Tried to get player target with invalid player");
		return nullptr;
	}

	return m_Player.Get();
}

bool UHSMAgentComponent::HasPath() const
{
	if (m_Owner.IsValid() == false)
	{
		LOG_ERROR("Tried to get owner HasPath with invalid owner");
		return true;
	}

	return m_Owner->HasPath();
}

void UHSMAgentComponent::SetFocus(AActor* target)
{
	if (m_Owner.IsValid() == false)
	{
		LOG_ERROR("Tried to set focus with invalid owner");
		return;
	}

	m_Owner->SetFocus(target);
}

void UHSMAgentComponent::ClearFocus()
{
	if (m_Owner.IsValid() == false)
	{
		LOG_ERROR("Tried to clear focus with invalid owner");
		return;
	}

	m_Owner->ClearFocus();
}

void UHSMAgentComponent::SetWaypointRoute(AWaypointRoute* route)
{
	m_WaypointRoute = route;

	if (m_CurrentState != EState::IDLE || m_WaypointRoute.IsValid() == false)
	{
		return;
	}

	if (m_States.Contains(m_CurrentState) && m_States[m_CurrentState] != nullptr)
	{
		m_States[m_CurrentState]->EndState();
		m_States[m_CurrentState]->StartState(this);
	}
}

AWaypointRoute* UHSMAgentComponent::GetWaypointRoute() const
{
	if(m_WaypointRoute.IsValid() == false)
	{
		return nullptr;
	}

	return m_WaypointRoute.Get();
}

void UHSMAgentComponent::SetVisionStrength(float visionStrength)
{
	m_VisionStrength = visionStrength;
}

const float UHSMAgentComponent::GetVisionStrength()
{
	return m_VisionStrength;
}

void UHSMAgentComponent::SetHearingStrength(float hearingStrength)
{
	m_HearingStrength = hearingStrength;
}

const float UHSMAgentComponent::GetHearingStrength()
{
	return m_HearingStrength;
}

bool UHSMAgentComponent::HasLineOfSight()
{
	return m_HasLineOfSight;
}

void UHSMAgentComponent::SetHasLineOfSight(bool hasLineOfSight)
{
	if (m_HasLineOfSight == hasLineOfSight || m_Player.IsValid() == false)
	{
		return;
	}

	m_HasLineOfSight = hasLineOfSight;
	m_HasLineOfSight ? m_Owner->SetFocus(m_Player.Get()) : m_Owner->ClearFocus();
}

bool UHSMAgentComponent::IsSoundHeard(float soundWeight) const
{
	LOG_ONSCREEN(-1, 2.0f, FColor::Yellow, "%f +++++++ %f", soundWeight, (1 - soundWeight));

	FString string = (m_HearingStrength > (1 - soundWeight)) ? "HEARD" : "NOT HEARD";
	LOG_ONSCREEN(-1, 2.0f, FColor::Yellow, "%s", *string);

	return (m_HearingStrength > (1 - soundWeight));
}

bool UHSMAgentComponent::IsInCombat() const
{
	return m_CurrentState == EState::COMBAT;
}

// Called when the game starts
void UHSMAgentComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UHSMAgentComponent::TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction)
{
	Super::TickComponent(deltaTime, tickType, thisTickFunction);

	if (m_States.Num() == 0 || m_States[m_CurrentState] == nullptr)
	{
		return;
	}

	m_States[m_CurrentState]->Update(deltaTime);
}

