// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Perception/Enums/SightBodyParts.h"
#include "AIHelperSubsystem.generated.h"

class ABaseCharacter;

UCLASS(Blueprintable)
class THEASCENDANCE_API UAIHelperSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	struct FTargetSocketInfo
	{
		TMap<FName, FVector> SocketLocations;
		float LastQueriedTime = 0.0f;
	};

public:
	FName GetBodyPartSocketName(ESightBodyParts bodyPart);
	float GetBodyPartSightWeight(ESightBodyParts bodyPart);

	FVector GetTargetSocketLocation(ABaseCharacter* target, FName socketName);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
private:
	void CleanupSocketInfoMap();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Sight", meta = (DisplayName = "Body Part Socket Names"))
	TMap<ESightBodyParts, FName> m_BodyPartSocketNames;

	UPROPERTY(EditDefaultsOnly, Category = "Sight", meta = (ClampMin = "0.0", ClampMax = "1.0", DisplayName = "Body Part Sight Weights"))
	TMap<ESightBodyParts, float> m_BodyPartSightWeights;

	const float m_SocketInfoLifetime = 5.0f;

	TMap<TWeakObjectPtr<ABaseCharacter>, FTargetSocketInfo> m_TargetSocketInfoMap;

	FTimerHandle m_SocketCleanupTimerHandle;
};
