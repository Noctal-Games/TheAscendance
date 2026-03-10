// Fill out your copyright notice in the Description page of Project Settings.


#include "KnockbackSpellDecorator.h"
#include "TheAscendance/Spells/Interfaces/SpellCaster.h"

#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"

void UKnockbackSpellDecorator::OnHit(AActor* hitActor, const FVector& spellHitLocation)
{
	if (m_ModifierData == nullptr)
	{
		LOG_ERROR("Knockback Spell Decorator modifier data is invalid")
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
