// Fill out your copyright notice in the Description page of Project Settings.


#include "ApproachCombatState.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/AI/Combat/Components/CombatAIComponent.h"
#include "TheAscendance/Characters/Enemies/BaseEnemy.h"

void UApproachCombatState::StartState()
{
	if (m_OwnerComponent != nullptr)
	{
		m_OwnerComponent->RequestWaitForState();
		m_OwnerComponent->SetIsAIMovementPaused(false);
		//Strafe speed
	}
	else
	{
		LOG_ERROR("[APPROACH COMBAT STATE] OwnerComponent is invalid");
	}
}

void UApproachCombatState::EndState()
{
	if (m_OwnerComponent != nullptr)
	{
		m_OwnerComponent->NotifyStateFinished();
		//Strafe speed
	}

	//Normal speed
	UAbstractCombatState::EndState();
}

void UApproachCombatState::Update(float deltaTime)
{
	if (m_OwnerComponent == nullptr)
	{
		LOG_ERROR("[APPROACH COMBAT STATE] OwnerComponent is invalid");
		return;
	}

	if(m_OwnerComponent->CombatContext.Target.IsValid() == false)
	{
		m_OwnerComponent->SetState(ECombatState::IDLE);
		return;
	}

	AActor* target = m_OwnerComponent->CombatContext.Target.Get();
	ABaseEnemy* ownerCharacter = Cast<ABaseEnemy>(m_OwnerComponent->GetOwner());

	const float distance = FVector::Distance(ownerCharacter->GetActorLocation(), target->GetActorLocation());
	const float maxRange = m_OwnerComponent->CombatSettings.MaxEngagementRange;

	if (distance <= maxRange)
	{
		m_OwnerComponent->NotifyStateFinished();
		return;
	}

	ownerCharacter->SetDestination(target->GetActorLocation());
}

const FString UApproachCombatState::GetStateToString() const
{
	return FString("APPROACH COMBAT STATE");
}
