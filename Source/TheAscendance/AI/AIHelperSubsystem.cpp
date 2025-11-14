// Fill out your copyright notice in the Description page of Project Settings.


#include "AIHelperSubsystem.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Characters/BaseCharacter.h"

FName UAIHelperSubsystem::GetBodyPartSocketName(ESightBodyParts bodyPart)
{
	if (const FName* name = m_BodyPartSocketNames.Find(bodyPart))
	{
		return *name;
	}

	LOG_ERROR("Tried to get socket name of an invalid body part");
	return FName("");
}

float UAIHelperSubsystem::GetBodyPartSightWeight(ESightBodyParts bodyPart)
{
	if (const float* weight = m_BodyPartSightWeights.Find(bodyPart))
	{
		return *weight;
	}

	LOG_ERROR("Tried to get sight weight of an invalid body part");
	return 0.0f;
}

FVector UAIHelperSubsystem::GetTargetSocketLocation(ABaseCharacter* target, FName socketName)
{
	if (target == nullptr)
	{
		LOG_ERROR("Tried to get socket location of an invalid target");
		return FVector::ZeroVector;
	}

	if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		const float currentTime = world->GetTimeSeconds();

		FTargetSocketInfo* socketInfo = m_TargetSocketInfoMap.Find(target);

		if (socketInfo == nullptr || currentTime - socketInfo->LastQueriedTime > 0.2f)
		{
			LOG_ONSCREEN(-1, 1.0f, FColor::Red, "UPDATING SOCKET INFO");

			FTargetSocketInfo& newSocketInfo = m_TargetSocketInfoMap.FindOrAdd(target);
			newSocketInfo.SocketLocations.Empty();

			for (const auto& pair : m_BodyPartSocketNames)
			{
				const FVector socketLocation = target->GetSocketLocation(pair.Value);
				newSocketInfo.SocketLocations.Add(pair.Value, socketLocation);
			}

			newSocketInfo.LastQueriedTime = currentTime;
			socketInfo = &newSocketInfo;
		}
		else
		{
			LOG_ONSCREEN(-1, 1.0f, FColor::Green, "USING CACHED SOCKET INFO");
		}

		if (const FVector* socketLocation = socketInfo->SocketLocations.Find(socketName))
		{
			return *socketLocation;
		}
		else
		{
			LOG_ERROR("Tried to get socket location of an invalid socket name");
		}
	}

	return FVector::ZeroVector;
}

void UAIHelperSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if(UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		world->GetTimerManager().SetTimer(m_SocketCleanupTimerHandle, this, &UAIHelperSubsystem::CleanupSocketInfoMap, 1.0f, true);
	}
}

void UAIHelperSubsystem::Deinitialize()
{
	if(UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		world->GetTimerManager().ClearTimer(m_SocketCleanupTimerHandle);
	}

	Super::Deinitialize();
}

bool UAIHelperSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (this->GetClass()->IsInBlueprint() && Super::ShouldCreateSubsystem(Outer))
	{
		return true;
	}

	return false;
}

void UAIHelperSubsystem::CleanupSocketInfoMap()
{
	if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		const float currentTime = world->GetTimeSeconds();

		int removedCount = 0;

		for (auto iterator = m_TargetSocketInfoMap.CreateIterator(); iterator; ++iterator)
		{
			if (iterator->Key.IsValid() == false || currentTime - iterator->Value.LastQueriedTime > m_SocketInfoLifetime)
			{
				iterator.RemoveCurrent();
				removedCount++;
			}
		}

		if (removedCount > 0)
		{
			LOG_INFO("Cleaned up %i expired socket info entries", removedCount);
		}
	}
}
