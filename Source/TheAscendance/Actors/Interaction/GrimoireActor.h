// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractableActor.h"
#include "GrimoireActor.generated.h"

/**
 * 
 */
UCLASS()
class THEASCENDANCE_API AGrimoireActor : public ABaseInteractableActor
{
	GENERATED_BODY()
	
public:
	virtual void Interact(class APlayerCharacter* player) override;
};
