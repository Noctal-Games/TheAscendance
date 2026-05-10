// Fill out your copyright notice in the Description page of Project Settings.


#include "ApproachCombatState.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/AI/Combat/Components/CombatAIComponent.h"
#include "TheAscendance/Characters/Enemies/BaseEnemy.h"

void UApproachCombatState::StartState()
{
}

void UApproachCombatState::EndState()
{
}

void UApproachCombatState::Update(float deltaTime)
{
	if(m_OwnerComponent->CombatContext.Target == nullptr)
	{
		m_OwnerComponent->SetState(ECombatState::IDLE);
		return;
	}

	AActor* target = m_OwnerComponent->CombatContext.Target.Get();
	ABaseEnemy* ownerCharacter = Cast<ABaseEnemy>(m_OwnerComponent->GetOwner());

	if (target == nullptr)
	{
		m_OwnerComponent->SetState(ECombatState::IDLE);
		return;
	}

	if(ownerCharacter == nullptr)
	{
		LOG_ERROR("[APPROACH COMBAT STATE] Owner is not of type ABaseEnemy");
		return;
	}

	const float distance = FVector::Distance(ownerCharacter->GetActorLocation(), target->GetActorLocation());
	const float preferredRange = 0.0f;// m_ClassData->PreferredEngagementRange;

	if (distance <= preferredRange)
	{
		m_OwnerComponent->SetState(ECombatState::ATTACK);
		return;
	}

	ownerCharacter->SetDestination(target->GetActorLocation());
}

const FString UApproachCombatState::GetStateToString() const
{
	return FString("APPROACH COMBAT STATE");
}
