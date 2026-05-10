// Fill out your copyright notice in the Description page of Project Settings.


#include "StreamableFunctionLibrary.h"

void UStreamableFunctionLibrary::RequestAsyncLoad(const FSoftObjectPath& targetToStream, TFunction<void()> delegate)
{
	LOG_INFO("[STREAMABLE FUNCTION LIBRARY] Requesting ASync Load for: %s", *targetToStream.ToString());

	UAssetManager::GetStreamableManager().RequestAsyncLoad(targetToStream, FStreamableDelegate::CreateLambda([delegate = MoveTemp(delegate), targetToStream]()
		{
			UObject* loadedObject = targetToStream.ResolveObject();

			if (loadedObject != nullptr)
			{
				LOG_INFO("[STREAMABLE FUNCTION LIBRARY] Successful ASync Load for: %s", *targetToStream.ToString());
			}
			else
			{
				LOG_ERROR("[STREAMABLE FUNCTION LIBRARY] Failed ASync Load for: %s", *targetToStream.ToString());
			}

			if (delegate.IsSet())
			{
				delegate();
			}
		}
	));
}

void UStreamableFunctionLibrary::RequestAsyncLoad(const TArray<FSoftObjectPath>& targetsToStream, TFunction<void()> delegate)
{
	if (targetsToStream.Num() == 0)
	{
		LOG_WARNING("[STREAMABLE FUNCTION LIBRARY] Tried to RequestASyncLoad for multiple items, but the array was empty");

		if (delegate.IsSet())
		{
			delegate();
		}

		return;
	}
	else if (targetsToStream.Num() == 1)
	{
		LOG_WARNING("[STREAMABLE FUNCTION LIBRARY] Requesting ASync Load for multiple items, but the array only contains one item. Consider using the single load alternative.");
	}

	for (const FSoftObjectPath& path : targetsToStream)
	{
		LOG_INFO("[STREAMABLE FUNCTION LIBRARY] Requesting ASync Load for: %s", *path.ToString());
	}

	UAssetManager::GetStreamableManager().RequestAsyncLoad(targetsToStream, FStreamableDelegate::CreateLambda([delegate = MoveTemp(delegate), targetsToStream]()
		{
			for (const FSoftObjectPath& path : targetsToStream)
			{
				UObject* loadedObject = path.ResolveObject();

				if (loadedObject != nullptr)
				{
					LOG_INFO("[STREAMABLE FUNCTION LIBRARY] Successful ASync Load for: %s", *path.ToString());
					continue;
				}

				LOG_ERROR("[STREAMABLE FUNCTION LIBRARY] Failed ASync Load for: %s", *path.ToString());
			}

			if (delegate.IsSet())
			{
				delegate();
			}
		}
	));
}
