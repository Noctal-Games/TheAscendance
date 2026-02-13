// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManagerSubsystem.h"

void UUIManagerSubsystem::Initialize(FSubsystemCollectionBase& collection)
{
	Super::Initialize(collection);
}

void UUIManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

bool UUIManagerSubsystem::ShouldCreateSubsystem(UObject* outer) const
{
	if (this->GetClass()->IsInBlueprint() && Super::ShouldCreateSubsystem(outer))
	{
		return true;
	}

	return false;
}
