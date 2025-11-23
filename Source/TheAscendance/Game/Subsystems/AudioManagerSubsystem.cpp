// Fill out your copyright notice in the Description page of Project Settings.


#include "AudioManagerSubsystem.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Characters/Enemies/BaseEnemy.h"
#include "TheAscendance/Characters/Player/PlayerCharacter.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void UAudioManagerSubsystem::LoadSoundAsset(TSoftObjectPtr<USoundBase> softSound, bool playSoundAfterLoad, FVector location, float volume)
{
	if (playSoundAfterLoad == false)
	{
		UCoreFunctionLibrary::RequestAsyncLoad(softSound.ToSoftObjectPath());
		return;
	}

	FSoftObjectPath path = softSound.ToSoftObjectPath();

	UCoreFunctionLibrary::RequestAsyncLoad(path, [this, path, location, volume]()
		{ 
			if (USoundBase* sound = Cast<USoundBase>(path.ResolveObject()))
			{
				PlaySoundAtLocation(sound, location, volume);
			}
		});
}

void UAudioManagerSubsystem::PlaySoundAtLocation(USoundBase* sound, FVector location, float volume, bool isNPCAudible)
{
	if (sound == nullptr)
	{
		LOG_ERROR("Tried to play invalid sound");
		return;
	}

	if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		LOG_WARNING("PLAYING SOUND");
		UGameplayStatics::PlaySoundAtLocation(world, sound, location, volume);

		if (isNPCAudible == true && sound->AttenuationSettings != nullptr)
		{
			LOG_WARNING("PLAYING SOUND FOR NPCS");
			PlaySoundForNPCPerception(sound->AttenuationSettings->Attenuation, location, volume);
		}
	}
}

UAudioComponent* UAudioManagerSubsystem::SpawnSoundAtLocation(bool spawnPlaying, USoundBase* sound, FVector location, FRotator rotation, float volume, bool autoDestroy)
{
	if (sound == nullptr)
	{
		LOG_ERROR("Tried to spawn invalid sound");
		return nullptr;
	}

	if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		UAudioComponent* audio = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), sound, location, rotation, volume, 0.0f, 0.0f, nullptr, nullptr, autoDestroy);

		if (audio == nullptr)
		{
			LOG_ERROR("SpawnSoundAtLocation failed");
			return nullptr;
		}

		InitAudioComponent(spawnPlaying, *audio);
		return audio;
	}

	return nullptr;
}

UAudioComponent* UAudioManagerSubsystem::SpawnSoundAttached(bool spawnPlaying, USceneComponent* owner, USoundBase* sound, float volume, bool autoDestroy, FName attachmentPoint, bool destroyWhenOwnerDestroyed)
{
	if (sound == nullptr)
	{
		LOG_ERROR("Tried to spawn invalid sound");
		return nullptr;
	}

	if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		UAudioComponent* audio = UGameplayStatics::SpawnSoundAttached(sound, owner, attachmentPoint, FVector::ZeroVector, EAttachLocation::SnapToTarget, destroyWhenOwnerDestroyed, volume, 1.0f, 0.0f, nullptr, nullptr, autoDestroy);

		if (audio == nullptr)
		{
			return nullptr;
		}

		InitAudioComponent(spawnPlaying, *audio);
		return audio;
	}

	return nullptr;
}

void UAudioManagerSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);
}

void UAudioManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UAudioManagerSubsystem::InitAudioComponent(bool spawnPlaying, UAudioComponent& component)
{
	spawnPlaying ? component.Play() : component.Stop();
}

void UAudioManagerSubsystem::PlaySoundForNPCPerception(const FSoundAttenuationSettings& sound, FVector location, float volume)
{
	UWorld* world = UCoreFunctionLibrary::GetGameWorld();

	if (world == nullptr)
	{
		return;
	}

	TArray<TEnumAsByte<EObjectTypeQuery>> types;
	types.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<TObjectPtr<AActor>> ignore;
	ignore.Add(UGameplayStatics::GetPlayerCharacter(world, 0));

	float range = sound.FalloffDistance * volume;

	TArray<AActor*> targets;

	UCoreFunctionLibrary::DrawDebugSphere(location, range, 16, FColor::Red);

	bool iHit = UKismetSystemLibrary::SphereOverlapActors(world, location, range, types, NULL, ignore, targets);

	if (iHit == false)
	{
		return;
	}

	for (TObjectPtr<AActor> actor : targets)
	{
		if (ABaseEnemy* enemy = Cast<ABaseEnemy>(actor))
		{
			if (enemy->IsInCombat() == true)
			{
				continue;
			}

			const float distSqr = FVector::DistSquared(location, enemy->GetActorLocation());

			if (distSqr > range * range)
			{
				continue;
			}

			const float dist = FMath::Sqrt(distSqr);
			float soundStrength = sound.AttenuationEval(dist, range);

			FCollisionQueryParams params;
			params.AddIgnoredActor(ignore[0]);

			FHitResult hit;

			if (world->LineTraceSingleByChannel(hit, location, enemy->GetActorLocation(), ECC_GameTraceChannel2, params) == false)
			{
				if (enemy->IsSoundHeard(soundStrength) == true)
				{
					enemy->SetLocationToInvestigate(location);
				}

				continue;
			}

			TArray<FHitResult> hits;

			world->LineTraceMultiByChannel(hits, location, enemy->GetActorLocation(), ECC_GameTraceChannel2, params);
			UCoreFunctionLibrary::DrawDebugLine(location, enemy->GetActorLocation(), FColor::Green, 5.0f);

			if (hits.Num() <= 1)
			{
				if (enemy->IsSoundHeard(soundStrength) == true)
				{
					enemy->SetLocationToInvestigate(location);
				}

				continue;
			}

			for (FHitResult block : hits)
			{
				if (block.GetActor() == nullptr)
				{
					continue;
				}

				if (block.GetActor() == enemy)
				{
					if (enemy->IsSoundHeard(soundStrength) == true)
					{
						enemy->SetLocationToInvestigate(location);
					}

					break;
				}

				const FBox bounds = block.GetActor()->GetComponentsBoundingBox(true);
				const FVector size = bounds.GetSize();
				const FVector boundsExtent = bounds.GetExtent();

				FVector rayDir = (enemy->GetActorLocation() - location).GetSafeNormal();

				const float thickness = FVector::DotProduct(boundsExtent * 2.0f, rayDir.GetAbs());
				const float crossSection = FMath::Max(size.X, size.Y) * size.Z;

				//Increase to make obstacle thickness less relevant
				const float thicknessDivisor = 200.0f;
				//Increase to make obstacle scale less relevant
				const float crossSectionDivisor = 50000.0f;

				const float thicknessFactor = FMath::Clamp(FMath::Sqrt(thickness / thicknessDivisor), 0.0f, 1.0f);
				const float crossSectionFactor = FMath::Clamp(FMath::Sqrt(crossSection / crossSectionDivisor), 0.0f, 1.0f);
				 
				//Increase multiplier for greater sound reduction
				const float obstructionLoss = (thicknessFactor + crossSectionFactor) * 0.2f;

				soundStrength -= obstructionLoss;

				if (soundStrength <= 0.0f)
				{
					break;
				}
			}

			if (enemy->IsSoundHeard(soundStrength) == true)
			{
				enemy->SetLocationToInvestigate(location);
			}
		}
	}
}

//float materialDamp = 0.7f;
//if (UPhysicalMaterial* Mat = hit.PhysMaterial.Get())
//{
//	switch (Mat->SurfaceType)
//	{
//	default: materialDamp = 0.6f; break;
//	}
//}

//const float obstructionLoss = (thicknessMultiplier * thicknessFactor) + (crossSectionMultiplier * crossSectionFactor) + (1.0f - materialDamp);


//for (FHitResult block : hits)
//{
//	if (block.GetActor() == nullptr)
//	{
//		continue;
//	}
//
//	if (block.GetActor() == enemy)
//	{
//		if (enemy->IsSoundHeard(soundStrength) == true)
//		{
//			//enemy->SetInvestigationLocation(location);
//		}
//		break;
//	}
//
//	const float thickness = (block.TraceStart - block.TraceEnd).Size();
//
//	const FBox bounds = block.GetActor()->GetComponentsBoundingBox(true);
//	const FVector size = bounds.GetSize();
//	const float crossSection = size.X * size.Z;
//
//	//Increase to make obstacle thickness less relevant
//	const float thicknessDivisor = 200.0f;
//	//Increase to make obstacle scale less relevant
//	const float crossSectionDivisor = 50000.0f;
//
//	const float thicknessFactor = FMath::Clamp(thickness / thicknessDivisor, 0.0f, 1.0f);
//	const float crossSectionFactor = FMath::Clamp(crossSection / crossSectionDivisor, 0.0f, 1.0f);
//
//	//Increase for greater obstruction loss
//	const float thicknessMultiplier = 0.5f;
//	//Increase if larger objects should block more sound
//	const float crossSectionMultiplier = 0.4f;
//
//	const float obstructionLoss = (thicknessMultiplier * thicknessFactor) + (crossSectionMultiplier * crossSectionFactor);
//
//	soundStrength -= obstructionLoss;
//
//	if (soundStrength <= 0.0f)
//	{
//		break;
//	}
//}


//hits.Sort([](const FHitResult& a, const FHitResult& b)
//	{
//		return a.Distance < b.Distance;
//	});
//
//int i = 0;
//
//
//while (i < hits.Num())
//{
//	LOG_ONSCREEN(-1, 2.0f, FColor::Red, "%i ++++++ %i", i, hits.Num());
//
//	const FHitResult& entryHit = hits[i];
//	AActor* hitActor = entryHit.GetActor();
//
//	if (hitActor == nullptr)
//	{
//		LOG_ONSCREEN(-1, 2.0f, FColor::Red, "HIT ACTOR IS NULL");
//
//		i++;
//		continue;
//	}
//
//	LOG_ONSCREEN(-1, 2.0f, FColor::Green, "%s", *hitActor->GetName());
//
//	if (hitActor == enemy)
//	{
//		if (enemy->IsSoundHeard(soundStrength) == true)
//		{
//			LOG_ONSCREEN(-1, 2.0f, FColor::Red, "HEARD 2");
//			//enemy->SetInvestigationLocation(location);
//		}
//
//		break;
//	}
//
//	float thickness = 1.0f;
//	int j;
//
//	for (j = i + 1; j < hits.Num(); j++)
//	{
//		LOG_ONSCREEN(-1, 2.0f, FColor::Green, "%s", *hits[j].GetActor()->GetName());
//
//		if (hits[j].GetActor() == hitActor)
//		{
//			thickness = hits[j].Distance - entryHit.Distance;
//			thickness = FMath::Max(thickness, 0.0f);
//			break;
//		}
//	}
//
//	const FBox bounds = hitActor->GetComponentsBoundingBox(true);
//	const FVector size = bounds.GetSize();
//	const float crossSection = FMath::Max(size.X, size.Y) * size.Z;
//
//	//Increase to make obstacle thickness less relevant
//	const float thicknessDivisor = 200.0f;
//	//Increase to make obstacle scale less relevant
//	const float crossSectionDivisor = 50000.0f;
//
//	const float thicknessFactor = FMath::Clamp(thickness / thicknessDivisor, 0.0f, 1.0f);
//	const float crossSectionFactor = FMath::Clamp(crossSection / crossSectionDivisor, 0.0f, 1.0f);
//
//	//Increase for greater obstruction loss
//	const float thicknessMultiplier = 0.5f;
//	//Increase if larger objects should block more sound
//	const float crossSectionMultiplier = 0.4f;
//
//	const float obstructionLoss = (thicknessMultiplier * thicknessFactor) + (crossSectionMultiplier * crossSectionFactor);
//
//	soundStrength -= obstructionLoss;
//
//	if (soundStrength <= 0.0f)
//	{
//		LOG_ONSCREEN(-1, 2.0f, FColor::Red, "BREAK");
//		break;
//	}
//
//	//Move to next hit after exit (or next hit if no exit found)
//	i = (j > i) ? j : i + 1;
//
//	LOG_ONSCREEN(-1, 2.0f, FColor::Red, "%i ++++++ %i", i, hits.Num());
//}
//
//if (enemy->IsSoundHeard(soundStrength) == true)
//{
//	//enemy->SetInvestigationLocation(location);
//}
//}