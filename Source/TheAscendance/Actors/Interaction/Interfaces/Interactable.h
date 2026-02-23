// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TheAscendance/Actors/Interaction/Enums/InteractType.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class APlayerCharacter;

class THEASCENDANCE_API IInteractable
{
	GENERATED_BODY()

public:
	virtual void Interact(APlayerCharacter* player) {};
	virtual EInteractType GetInteractType() { return EInteractType::INTERACT; };
};
