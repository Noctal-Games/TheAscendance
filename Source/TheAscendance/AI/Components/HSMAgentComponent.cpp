// Fill out your copyright notice in the Description page of Project Settings.


#include "HSMAgentComponent.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/AI/States/IdleState.h"
#include "TheAscendance/AI/States/InvestigateState.h"
#include "TheAscendance/AI/States/CombatState.h"
#include "TheAscendance/AI/Components/PerceptionComponent.h"
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

void UHSMAgentComponent::Init(ABaseEnemy* owner, UEnemyClassData* classData, const FBehaviourSettings& behaviourSettings, const FPerceptionSettings& perceptionSettings, const FCombatSettings& combatSettings)
{
	if (owner == nullptr)
	{
		LOG_ERROR("[HSM AGENT COMPONENT] Tried to Init with invalid owner");
		return;
	}

	if (classData == nullptr)
	{
		LOG_ERROR("[HSM AGENT COMPONENT] Tried to Init with invalid class data");
		//return;
	}

	m_ClassData = classData;

	m_Owner = owner;
	m_Player = UCoreFunctionLibrary::GetPlayerCharacter();

	if(m_Player.IsValid() == false)
	{
		LOG_ERROR("[HSM AGENT COMPONENT] Failed to get valid player reference");
		return;
	}

	m_CurrentState = EState::MAX;
	m_States.Add(EState::IDLE, NewObject<UIdleState>());
	m_States.Add(EState::INVESTIGATE, NewObject<UInvestigateState>());

	if (UCombatState* combatState = NewObject<UCombatState>())
	{
		FLoadedCombatSettings  loadedCombatSettings;
		loadedCombatSettings.Abilities = m_AbilityData;
		loadedCombatSettings.GoalWeights = combatSettings.GoalWeights;
		combatState->Init(loadedCombatSettings);
		m_States.Add(EState::COMBAT, combatState);
	}

	m_PerceptionComponent = NewObject<UPerceptionComponent>(m_Owner.Get(), TEXT("SIGHT SENSOR"));
	m_PerceptionComponent->RegisterComponent();
	m_PerceptionComponent->SetTarget(m_Player.Get());
	m_PerceptionComponent->Init(perceptionSettings);
	m_PerceptionComponent->SetIsActive(true);

	m_BehaviourSettings = behaviourSettings;

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

bool UHSMAgentComponent::IsTargetInActionableRange(const FVector& target) const
{
	if (m_Owner == nullptr || m_ClassData == nullptr)
	{
		return false;
	}

	return ((m_Owner->GetActorLocation() - target).Length() <= m_ClassData->PreferredEngagementRange + (m_ClassData->EngagementRangeTolerance * 2.0f));
}

void UHSMAgentComponent::GetPreferredDistanceValues(float& preferredDistanceFromTarget, float& preferredDistanceTolerance) const
{
	preferredDistanceFromTarget = m_ClassData->PreferredEngagementRange;
	preferredDistanceTolerance = m_ClassData->EngagementRangeTolerance;
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

float UHSMAgentComponent::GetRandomCombatReactionTime() const
{
	return m_BehaviourSettings.ReactionTime.GetRandomValue();
}

bool UHSMAgentComponent::IsTargetDetected() const
{
	if (m_PerceptionComponent == nullptr)
	{
		LOG_ERROR("[HSM AGENT COMPONENT] Tried to get HasLineOfSight with invalid perception component");
		return false;
	}

	return m_PerceptionComponent->IsTargetDetected();
}


bool UHSMAgentComponent::HasLineOfSight() const
{
	if(m_PerceptionComponent == nullptr)
	{
		LOG_ERROR("[HSM AGENT COMPONENT] Tried to get HasLineOfSight with invalid perception component");
		return false;
	}

	return m_PerceptionComponent->HasLineOfSight();
}

bool UHSMAgentComponent::IsSoundHeard(const float soundWeight) const
{
	if (m_PerceptionComponent == nullptr)
	{
		LOG_ERROR("[HSM AGENT COMPONENT] Tried to get HasLineOfSight with invalid perception component");
		return false;
	}

	return m_PerceptionComponent->IsSoundHeard(soundWeight);
}

bool UHSMAgentComponent::IsInCombat() const
{
	return m_CurrentState == EState::COMBAT;
}

bool UHSMAgentComponent::IsTargetTooClose(const FVector& target) const
{
	if(m_Owner.IsValid() == false || m_ClassData == nullptr)
	{
		LOG_ERROR("[HSM AGENT COMPONENT] Tried to check IsTargetTooClose with invalid owner");
		return false;
	}

	FVector distance = target - m_Owner->GetActorLocation();
	return distance.Length() <= (m_ClassData->PreferredEngagementRange - m_ClassData->EngagementRangeTolerance);
}

bool UHSMAgentComponent::IsTargetTooFar(const FVector& target) const
{
	if (m_Owner.IsValid() == false || m_ClassData == nullptr)
	{
		LOG_ERROR("Tried to check if target is too fase with invalid owner");
		return false;
	}

	FVector distance = target - m_Owner->GetActorLocation();
	return distance.Length() > (m_ClassData->PreferredEngagementRange + m_ClassData->EngagementRangeTolerance);
}

void UHSMAgentComponent::AddAbility(const FEnemyLoadedAbilityData& abilityData)
{
	m_AbilityData.Add(abilityData);
}

bool UHSMAgentComponent::TryConsumeReaction()
{
	UWorld* world = UCoreFunctionLibrary::GetGameWorld();

	if (world == nullptr)
	{
		return false;
	}

	const float currentTime = world->GetTimeSeconds();
	bool canReact = (currentTime - m_LastReactionTime) >= m_CurrentReactionTime;

	if (canReact == false)
	{
		return false;
	}

	m_LastReactionTime = currentTime;
	m_CurrentReactionTime = m_BehaviourSettings.ReactionTime.GetRandomValue();
	return true;
}

const FBehaviourSettings& UHSMAgentComponent::GetBehaviourSettings()
{
	return m_BehaviourSettings;
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

	LOG_ONSCREEN(4, 2.0f, FColor::Yellow, "CURRENT STATE: %s", *m_States[m_CurrentState]->GetStateToString());
	m_States[m_CurrentState]->Update(deltaTime);
}

