// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TheAscendance/Actors/Interaction/BaseInteractableActor.h"
#include "TheAscendance/Actors/Interaction/Interfaces/Interactable.h"
#include "Item.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class APlayerCharacter;
class UItemData;

UCLASS()
class THEASCENDANCE_API AItem : public ABaseInteractableActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	virtual void Init(UItemData* itemData);
	void SetStaticMesh();

	virtual void Interact(APlayerCharacter* player) override;
	virtual EInteractType GetInteractType() override;
protected:
	UPROPERTY();
	TSoftObjectPtr<UStaticMesh> m_Mesh = nullptr;
	UPROPERTY(EditInstanceOnly, meta = (DisplayName = "Static Mesh Component"));
	TObjectPtr<UStaticMeshComponent> m_MeshComponent = nullptr;

	UPROPERTY();
	TObjectPtr<UItemData> m_ItemData = nullptr;
};
