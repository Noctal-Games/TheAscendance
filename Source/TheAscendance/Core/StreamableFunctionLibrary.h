// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "UObject/NoExportTypes.h"
#include "StreamableFunctionLibrary.generated.h"

UCLASS()
class THEASCENDANCE_API UStreamableFunctionLibrary : public UObject
{
	GENERATED_BODY()
	
public:
	template<typename T>
	static T* LoadAsset(const FSoftObjectPath& targetToStream)
	{
		if (targetToStream.IsValid() == false)
		{
			return nullptr;
		}

		if(UObject* loadedObject = UAssetManager::GetStreamableManager().LoadSynchronous(targetToStream))
		{
			LOG_INFO("[STREAMABLE FUNCTION LIBRARY] Successful sync load for: %s", *targetToStream.ToString());
			return Cast<T>(loadedObject);

		}

		LOG_ERROR("[STREAMABLE FUNCTION LIBRARY] Failed sync load for: %s", *targetToStream.ToString());
		return nullptr;
	}

	static void RequestAsyncLoad(const FSoftObjectPath& targetToStream, TFunction<void()> delegate = nullptr);
	static void RequestAsyncLoad(const TArray<FSoftObjectPath>& targetsToStream, TFunction<void()> delegate = nullptr);
};
