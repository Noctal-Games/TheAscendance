// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInteractableActor.h"
#include "TheAscendance/Core/CoreMacros.h"

// Sets default values
ABaseInteractableActor::ABaseInteractableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ABaseInteractableActor::Interact(APlayerCharacter* player)
{
	LOG_INFO("[INTERACTABLE ACTOR] Interacted with: %s", *GetName());

	if (OnInteract.IsBound() == true)
	{
		OnInteract.Broadcast();
	}
}

EInteractType ABaseInteractableActor::GetInteractType()
{
	return EInteractType::INTERACT;
}

// Called when the game starts or when spawned
void ABaseInteractableActor::BeginPlay()
{
	Super::BeginPlay();
	
}