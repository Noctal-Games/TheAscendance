// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeldEquippableItem.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class UWeaponItemData;

UCLASS()
class THEASCENDANCE_API AHeldEquippableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHeldEquippableItem();

	virtual void Init(UWeaponItemData* itemData);
	void UnEquip();

	const UWeaponItemData* GetItemData() const;

private:
	void SetStaticMesh();

protected:
	UPROPERTY();
	TObjectPtr<UWeaponItemData> m_ItemData = nullptr;

	UPROPERTY();
	TObjectPtr<UStaticMeshComponent> m_MeshComponent = nullptr;

	UPROPERTY();
	TSoftObjectPtr<UStaticMesh> m_Mesh = nullptr;
};
