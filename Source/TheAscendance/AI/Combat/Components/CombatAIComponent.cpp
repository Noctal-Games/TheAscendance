// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatAIComponent.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/AI/Combat/States/AbstractCombatState.h"
#include "TheAscendance/AI/Combat/States/IdleCombatState.h"
#include "TheAscendance/AI/Combat/States/ApproachCombatState.h"
#include "TheAscendance/AI/Combat/States/StrafeCombatState.h"
#include "TheAscendance/AI/Combat/States/RetreatCombatState.h"
#include "TheAscendance/AI/States/AttackCombatState.h"
#include "TheAscendance/Characters/Enemies/BaseEnemy.h"
#include "TheAscendance/Abilities/Components/AbilityComponent.h"
#include "TheAscendance/AI/TAAIController.h"
#include "TheAscendance/AI/Components/PerceptionComponent.h"

// Sets default values for this component's properties
UCombatAIComponent::UCombatAIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UCombatAIComponent::Init(UPerceptionComponent* perceptionComponent, const FLoadedCombatSettings& combatSettings, UAbilityComponent* abilityComponent)
{
	if (abilityComponent != nullptr)
	{
		abilityComponent->OnAbilityFinished.AddUObject(this, &UCombatAIComponent::HandleAbilityFinished);
	}

	m_PerceptionComponent = perceptionComponent;

	if (m_PerceptionComponent != nullptr)
	{
		m_PerceptionComponent->SetIsActive(true);
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

	if (UStrafeCombatState* state = NewObject<UStrafeCombatState>())
	{
		state->Init(this);
		m_CombatStates.Add(ECombatState::STRAFE, state);
	}

	if (URetreatCombatState* state = NewObject<URetreatCombatState>())
	{
		state->Init(this);
		m_CombatStates.Add(ECombatState::RETREAT, state);
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
	//Can assume that locking combat pauses AI movement and focus for now, as currently only used by abilities and ability movement will be montage/root motion driven

	if (val == true)
	{
		SetIsAIMovementPaused(true);
		SetFocus(nullptr);
	}
	else
	{
		SetIsAIMovementPaused(false);

		if (CombatContext.Target.IsValid())
		{
			SetFocus(CombatContext.Target.Get());
		}
	}

	m_IsCombatLocked = val;
}

void UCombatAIComponent::SetIsAIMovementPaused(bool val)
{
	if (m_Controller.IsValid() == false)
	{
		LOG_ERROR("[COMBAT AI COMPONENT] Controller is invalid");
		return;
	}

	m_Controller->SetPauseMovement(val);
}

void UCombatAIComponent::SetFocus(AActor* target)
{
	if (m_Controller.IsValid() == false)
	{
		LOG_ERROR("[BASE ENEMY] Tried to set focus with invalid controller");
		return;
	}

	if (target == nullptr)
	{
		m_Controller->ClearFocus(EAIFocusPriority::Gameplay);
		return;
	}

	m_Controller->SetFocus(target);
}

void UCombatAIComponent::LookAtTarget()
{
	if (m_Controller.IsValid() == false)
	{
		LOG_ERROR("[COMBAT AI COMPONENT] Tried to set focus with invalid controller");
		return;
	}

	if (CombatContext.Target == nullptr)
	{
		return;
	}

	m_Controller->SetFocus(CombatContext.Target.Get());
}

void UCombatAIComponent::RequestWaitForState()
{
	m_WaitForState = true;
	m_StateFinished = false;
}

void UCombatAIComponent::NotifyStateFinished()
{
	m_StateFinished = true;
}

void UCombatAIComponent::SetDestination(const FVector& destination)
{
	if (m_Controller.IsValid() == false)
	{
		LOG_ERROR("[COMBAT AI COMPONENT] Tried to set destination with invalid controller");
		return;
	}

	m_Controller->SetDestination(destination);
}

void UCombatAIComponent::GetCombatRangeValues(float& PreferredEngagementRange, float& MaxEngagementRange, float& EngagementRangeTolerance)
{
	PreferredEngagementRange = CombatSettings.PreferredEngagementRange;
	MaxEngagementRange = CombatSettings.MaxEngagementRange;
	EngagementRangeTolerance = CombatSettings.EngagementRangeTolerance;
}

float UCombatAIComponent::GetAggression()
{
	return BehaviourSettings.Aggression;
}

ABaseEnemy* UCombatAIComponent::GetEnemyOwner()
{
	if (m_Owner.IsValid() == false)
	{
		return nullptr;
	}

	return m_Owner.Get();
}

AActor* UCombatAIComponent::GetCurrentTarget()
{
	if (CombatContext.Target.IsValid() == false)
	{
		return nullptr;
	}

	return CombatContext.Target.Get();
}

// Called when the game starts
void UCombatAIComponent::BeginPlay()
{
	Super::BeginPlay();

	m_Owner = Cast<ABaseEnemy>(GetOwner());

	if (m_Owner.IsValid() == true)
	{
		m_Controller = Cast<ATAAIController>(m_Owner->GetController());
	}
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

	if (m_WaitForState == true)
	{
		LOG_ONSCREEN(5, 2.0f, FColor::Yellow, "WAITNG FOR STATE, STATE FINISHED IS %s", *FString(m_StateFinished ? "TRUE" : "FALSE"));

		if (m_StateFinished == false)
		{
			ApplyMovement(deltaTime);
			return;
		}

		m_WaitForState = false;
		m_StateFinished = false;
	}

	if (TryConsumeReaction() == true)
	{
		if (m_PerceptionComponent == nullptr)
		{
			LOG_ERROR("[COMBAT AI COMPONENT] PerceptionComponent is invalid");
			return;
		}

		EvaluateCombat();
	}

	ApplyMovement(deltaTime);
}

void UCombatAIComponent::EvaluateCombat()
{
	float highestScore = -1.0f;
	ECombatState desiredState = ECombatState::IDLE;

	//Setup perception to monitor player and allies and query targets here
	//Temp target only player
	CombatContext.Target = UCoreFunctionLibrary::GetPlayerCharacter();

	if (CombatContext.Target.IsValid() == false)
	{
		SetState(ECombatState::IDLE);
		return;
	}

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
		desiredState = ECombatState::STRAFE;
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

	//Change to ScoreTooFar, which would encompass Approaching.... Etc
	if (tooFar == true)
	{
		SetState(ECombatState::APPROACH);
		return;
	}

	//Change to ScoreTooClose, which would encompass Retreating, Holding Ground.... Etc
	const float retreatScore = ScoreRetreat();

	if (retreatScore > highestScore)
	{
		highestScore = retreatScore;
		desiredState = ECombatState::RETREAT;
	}

	SetState(desiredState);
}

void UCombatAIComponent::HandleAbilityFinished()
{
	SetIsCombatLocked(false);
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

	if (score == 0.0f)
	{
		return -1.0f;
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

	if (score == 0.0f)
	{
		return -1.0f;
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
	float score = 0.0f;

	if (CombatContext.Target.IsValid() == false)
	{
		return -1.0f;
	}

	AActor* target = CombatContext.Target.Get();

	const float distance = (target->GetActorLocation() - GetOwner()->GetActorLocation()).Length();
	const float preferredRange = CombatSettings.PreferredEngagementRange;

	const float rangeDelta = distance - preferredRange; 
	const float tooClose = FMath::Clamp(-rangeDelta / preferredRange, 0.0f, 1.0f);

	const float aggression = BehaviourSettings.Aggression;

	const float defensiveWeight = 1.0f - aggression;

	score = tooClose * defensiveWeight;

	if (score == 0.0f)
	{
		return -1.0f;
	}

	return score;
}

float UCombatAIComponent::ScoreStrafe() const
{
	float score = 0.0f;

	if (CombatContext.Target.IsValid() == false)
	{
		LOG_WARNING("TARGET INVALID");
		return score;
	}

	AActor* target = CombatContext.Target.Get();

	const float distance = (target->GetActorLocation() - GetOwner()->GetActorLocation()).Length();
	const float preferredRange = CombatSettings.PreferredEngagementRange;

	const float rangeDelta = distance - preferredRange;

	// Strongest when far from ideal range
	const float distanceRatio = FMath::Abs(rangeDelta) / FMath::Max(preferredRange, 1.0f);
	const float rangeUrgency = FMath::Clamp(distanceRatio, 0.0f, 1.0f);

	const float aggression = BehaviourSettings.Aggression;

	const float defensiveWeight = 1.0f - BehaviourSettings.Aggression;

	const float rangeTolerance = CombatSettings.EngagementRangeTolerance;
	const float discipline = 1.0f - rangeTolerance;
	//const float randomFactor = m_BehaviourSettings.DecisionRandomness.GetRandomValue();

	score = rangeUrgency * defensiveWeight * discipline;

	if (rangeUrgency < 0.1f)
	{
		score *= 0.2f;
	}

	if (score == 0.0f)
	{
		return -1.0f;
	}

	return score;
}

bool UCombatAIComponent::TryConsumeReaction()
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
	m_CurrentReactionTime = BehaviourSettings.ReactionTime.GetRandomValue();
	return true;
}

void UCombatAIComponent::SmoothMovementIntent(float deltaTime)
{
	const float smoothingSpeed = 7.0f;

	const float alpha = 1.0f - FMath::Exp(-smoothingSpeed * deltaTime);

	m_SmoothedMovement = FMath::Lerp(m_SmoothedMovement, m_MovementIntent, alpha);

	m_SmoothedMovement.Z = 0.0f;
	m_SmoothedMovement.Normalize();
}

void UCombatAIComponent::ApplyMovement(float deltaTime)
{
	AActor* owner = GetOwner();

	if (owner == nullptr)
	{
		return;
	}

	SmoothMovementIntent(deltaTime);

	const FVector ownerLocation = owner->GetActorLocation();
	const FVector destination = ownerLocation + (m_SmoothedMovement * 300.0f);

	SetDestination(destination);
}