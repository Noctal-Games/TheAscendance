// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseDeliveryEffect.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/GameplayTagHelpers.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Effects/Structs/EffectData.h"
#include "TheAscendance/Effects/CoreEffect.h"
#include "TheAscendance/Characters/Interfaces/Susceptible.h"

#include "Kismet/KismetSystemLibrary.h"

void UBaseDeliveryEffect::ApplyEffect(ISusceptible* target)
{
	if (target == nullptr || m_EffectToApply == nullptr)
	{
		return;
	}

	if (target->IsDead() == true || target->HasImmunity(m_EffectToApply->EffectTag) == true)
	{
		return;
	}

	if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
	{
		UBaseEffect* casterEffect = gameMode->CreateEffectFromEffectData(m_EffectToApply.Get());

		if (casterEffect == nullptr)
		{
			return;
		}

		if (UCoreEffect* coreEffect = Cast<UCoreEffect>(casterEffect))
		{
			target->AddEffect(coreEffect);
		}
	}
}

void UBaseDeliveryEffect::ScanForTargets(const FVector& origin, float radius, TArray<AActor*>& targets)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> types;
	types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));
	types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<TObjectPtr<AActor>> ignore;
	//ignore.Add(owner);

	TArray<AActor*> actors;

	UCoreFunctionLibrary::DrawDebugSphere(origin, radius, 16, FColor::Yellow);

	if (UKismetSystemLibrary::SphereOverlapActors(UCoreFunctionLibrary::GetGameWorld(), origin, radius, types, NULL, ignore, actors))
	{
		for (TObjectPtr<AActor> actor : actors)
		{
			targets.Add(actor);
		}
	}
}