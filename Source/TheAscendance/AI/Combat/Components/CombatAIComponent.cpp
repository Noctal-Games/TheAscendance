// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAIComponent.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/AI/Combat/States/AbstractCombatState.h"
#include "TheAscendance/AI/Combat/States/IdleCombatState.h"
#include "TheAscendance/AI/Combat/States/ApproachCombatState.h"
#include "TheAscendance/AI/States/AttackCombatState.h"
#include "TheAscendance/Characters/Enemies/BaseEnemy.h"
#include "TheAscendance/Abilities/Components/AbilityComponent.h"

// Sets default values for this component's properties
UCombatAIComponent::UCombatAIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCombatAIComponent::Init(const FLoadedCombatSettings& combatSettings, UAbilityComponent* abilityComponent)
{
	if (abilityComponent != nullptr)
	{
		abilityComponent->OnAbilityFinished.AddUObject(this, &UCombatAIComponent::HandleAbilityFinished);
	}

	CombatSettings = combatSettings;

	m_CurrentCombatState = ECombatState::MAX;

	if (UIdleCombatState* state = NewObject<UIdleCombatState>())
	{
		state->Init(this);
		m_CombatStates.Add(ECombatState::IDLE, state);
	}

	if (UApproachCombatState* state = NewObject<UApproachCombatState>())
	{
		state->Init(this);
		m_CombatStates.Add(ECombatState::APPROACH, state);
	}

	if (UAttackCombatState* state = NewObject<UAttackCombatState>())
	{
		state->Init(this);
		m_CombatStates.Add(ECombatState::ATTACK, state);
	}

	if (m_CombatStates.Num() != (int32)EState::MAX)
	{
		LOG_ERROR("[COMBAT STATE] CombatStates count does not match ECombatState length");
	}

	SetState(ECombatState::IDLE);
}

void UCombatAIComponent::SetState(ECombatState newState)
{
	if (m_CurrentCombatState == newState)
	{
		return;
	}
	else if (m_CombatStates.Contains(newState) == false || m_CombatStates[newState] == nullptr)
	{
		LOG_ERROR("[COMBAT STATE] Tried to set CombatState to an invalid state");
		return;
	}

	if (m_CombatStates.Contains(m_CurrentCombatState) && m_CombatStates[m_CurrentCombatState] != nullptr)
	{
		m_CombatStates[m_CurrentCombatState]->EndState();
	}

	m_CurrentCombatState = newState;
	m_CombatStates[m_CurrentCombatState]->StartState();
}

void UCombatAIComponent::UseAbility()
{
	//Temp

	for (const FLoadedAbilityData& Ability : CombatSettings.Abilities)
	{
		if (m_Owner.IsValid() == false || m_Owner->IsAbilityOnCooldown(Ability.AbilityTag) == true)
		{
			continue;
		}

		m_Owner->StartAbility(Ability.AbilityTag);
	}
}

void UCombatAIComponent::SetIsCombatLocked(bool val)
{
	m_IsCombatLocked = val;
}

// Called when the game starts
void UCombatAIComponent::BeginPlay()
{
	Super::BeginPlay();

	m_Owner = Cast<ABaseEnemy>(GetOwner());
}

// Called every frame
void UCombatAIComponent::TickComponent(float deltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(deltaTime, TickType, ThisTickFunction);

	if (m_CombatStates.Num() != 0 && m_CombatStates[m_CurrentCombatState] != nullptr)
	{
		m_CombatStates[m_CurrentCombatState]->Update(deltaTime);
		LOG_ONSCREEN(4, 2.0f, FColor::Yellow, "CURRENT STATE: %s", *m_CombatStates[m_CurrentCombatState]->GetStateToString());
	}

	if (m_IsCombatLocked == true)
	{
		return;
	}

	m_DecisionTimer -= deltaTime;

	if (m_DecisionTimer <= 0.0f)
	{
		EvaluateCombat();
		//ResetDecisionTimer();
	}
}

void UCombatAIComponent::EvaluateCombat()
{
	float highestScore = -9999.0f;
	ECombatState desiredState = ECombatState::IDLE;

	//Setup perception to monitor player and allies and query targets here
	//Temp target only player
	CombatContext.Target = UCoreFunctionLibrary::GetPlayerCharacter();

	const float attackScore = ScoreAttack();

	if (attackScore > highestScore)
	{
		highestScore = attackScore;
		desiredState = ECombatState::ATTACK;
	}

	const float supportScore = ScoreSupport();

	if (supportScore > highestScore)
	{
		highestScore = supportScore;
		//desiredState = ECombatState::SUPPORT;
	}

	const float selfScore = ScoreSelf();

	if (selfScore > highestScore)
	{
		highestScore = selfScore;
		//desiredState = ECombatState::PRESERVE_SELF;
	}

	const float strafeScore = ScoreStrafe();

	if (strafeScore > highestScore)
	{
		highestScore = strafeScore;
		//desiredState = ECombatState::IDLE;
	}

	if (CombatContext.Target.IsValid() == false)
	{
		SetState(ECombatState::IDLE);
		return;
	}

	const float distance = (GetOwner()->GetActorLocation() - CombatContext.Target->GetActorLocation()).Length();
	CombatContext.DistanceToTarget = distance;

	const float maxRange = CombatSettings.MaxEngagementRange;

	if (distance > maxRange)
	{
		SetState(ECombatState::APPROACH);
		return;
	}

	const float preferredRange = CombatSettings.PreferredEngagementRange;
	const float tolerance = CombatSettings.EngagementRangeTolerance;

	const float rangeDelta = distance - preferredRange;

	const bool tooFar = rangeDelta > (preferredRange * tolerance);

	if (tooFar == true)
	{
		SetState(ECombatState::APPROACH);
		return;
	}

	const bool tooClose = rangeDelta < -(preferredRange * tolerance);

	if (tooClose == true)
	{
		const float retreatScore = ScoreRetreat();

		if (retreatScore > highestScore)
		{
			//SetState(ECombatState::Retreat);
			return;
		}
	}

	SetState(desiredState);
}

void UCombatAIComponent::HandleAbilityFinished()
{
	m_IsCombatLocked = false;
	SetState(ECombatState::IDLE);
	//SetState(ECombatState::Retreat);
}

float UCombatAIComponent::ScoreAttack() const
{
	float score = 0.0f;
	
	for (const FLoadedAbilityData& Ability : CombatSettings.Abilities)
	{
		if (m_Owner.IsValid() == true && m_Owner->IsAbilityOnCooldown(Ability.AbilityTag) == true)
		{
			continue;
		}

		for (EAbilityGoal goal : Ability.Goals)
		{
			if (goal == EAbilityGoal::DAMAGE || goal == EAbilityGoal::LAST_RESORT_DAMAGE || goal == EAbilityGoal::DEBUFF)
			{
				const float goalWeight = CombatSettings.GoalWeights.FindRef(goal);
				score += (Ability.Weight + 1) * (goalWeight + 1);
			}
		}
	}

	return score;
}

float UCombatAIComponent::ScoreSupport() const
{
	float score = 0.0f;

	for (const FLoadedAbilityData& Ability : CombatSettings.Abilities)
	{
		if (m_Owner.IsValid() == true && m_Owner->IsAbilityOnCooldown(Ability.AbilityTag) == true)
		{
			continue;
		}

		for (EAbilityGoal goal : Ability.Goals)
		{
			if (goal == EAbilityGoal::HEAL_ALLY || goal == EAbilityGoal::BUFF_ALLY)
			{
				const float goalWeight = CombatSettings.GoalWeights.FindRef(goal);
				score += (Ability.Weight + 1) * (goalWeight + 1);
			}
		}
	}

	return score;
}

float UCombatAIComponent::ScoreSelf() const
{
	float score = 0.0f;

	for (const FLoadedAbilityData& Ability : CombatSettings.Abilities)
	{
		if (m_Owner.IsValid() == true && m_Owner->IsAbilityOnCooldown(Ability.AbilityTag) == true)
		{
			continue;
		}

		for (EAbilityGoal goal : Ability.Goals)
		{
			if (goal == EAbilityGoal::HEAL_SELF || goal == EAbilityGoal::BUFF_SELF)
			{
				const float goalWeight = CombatSettings.GoalWeights.FindRef(goal);
				score += (Ability.Weight + 1) * (goalWeight + 1);
			}
		}
	}

	return score;
}

float UCombatAIComponent::ScoreRetreat() const
{
	return 0.0f;
}

float UCombatAIComponent::ScoreStrafe() const
{
	return 0.0f;
}




