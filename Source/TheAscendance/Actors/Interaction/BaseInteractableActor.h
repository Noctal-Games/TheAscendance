// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "BaseInteractableActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);

UCLASS()
class THEASCENDANCE_API ABaseInteractableActor : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseInteractableActor();

	virtual void Interact(class APlayerCharacter* player) override;
	virtual EInteractType GetInteractType() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnInteract OnInteract;
};
