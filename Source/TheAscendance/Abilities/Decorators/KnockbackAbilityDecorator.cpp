// Fill out your copyright notice in the Description page of Project Settings.


#include "KnockbackAbilityDecorator.h"

#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"

void UKnockbackAbilityDecorator::OnHit(AActor* hitActor, const FVector& spellHitLocation)
{
	if (m_ModifierData == nullptr)
	{
		LOG_ERROR("[KNOCKBACK ABILITY DECORATOR] Modifier data is invalid")
		return;
	}

	FVector knockbackDirection = hitActor->GetActorLocation() - spellHitLocation;
	knockbackDirection.Normalize();
	knockbackDirection.Z = FMath::Max(knockbackDirection.Z, 0.5f);

	if (ACharacter* character = Cast<ACharacter>(hitActor))
	{
		character->LaunchCharacter(knockbackDirection * m_ModifierData->KnockbackStrength, true, true);
		return;
	}

	if (UPrimitiveComponent* primitiveComponent = hitActor->FindComponentByClass<UPrimitiveComponent>())
	{
		if (primitiveComponent->IsSimulatingPhysics() == false)
		{
			return;
		}

		primitiveComponent->AddImpulse(knockbackDirection * m_ModifierData->KnockbackStrength, NAME_None, true);
	}
}
