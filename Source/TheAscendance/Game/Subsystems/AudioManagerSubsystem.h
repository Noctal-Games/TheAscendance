// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "AudioManagerSubsystem.generated.h"

class USoundBase;
class UAudioComponent;
class USceneComponent;

UCLASS()
class THEASCENDANCE_API UAudioManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void LoadSoundAsset(TSoftObjectPtr<USoundBase> softSound, bool playSoundAfterLoad = false, FVector location = FVector::ZeroVector, float volume = 1.0f);
	UFUNCTION(BlueprintCallable)
	void PlaySoundAtLocation(USoundBase* sound, FVector location, float volume, bool isNPCAudible = false);

	UFUNCTION(BlueprintCallable)
	UAudioComponent* SpawnSoundAtLocation(bool spawnPlaying, USoundBase* sound, FVector location, FRotator rotation, float volume, bool autoDestroy);
	UFUNCTION(BlueprintCallable)
	UAudioComponent* SpawnSoundAttached(bool spawnPlaying, USceneComponent* owner, USoundBase* sound, float volume, bool autoDestroy, FName attachmentPoint = "", bool destroyWhenOwnerDestroyed = true);

	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

private:
	void InitAudioComponent(bool spawnPlaying, UAudioComponent& component);
	void PlaySoundForNPCPerception(const FSoundAttenuationSettings& sound, FVector location, float volume);
};
