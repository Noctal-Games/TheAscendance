// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeldItem.generated.h"

class UStaticMeshComponent;
class UStaticMesh;
class UWeaponItemData;

UCLASS()
class THEASCENDANCE_API AHeldItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AHeldItem();

	virtual void Init(UWeaponItemData* itemData);
	void UnEquip();

private:
	void SetStaticMesh();

private:
	UPROPERTY();
	TObjectPtr<UWeaponItemData> m_ItemData = nullptr;

	UPROPERTY();
	TObjectPtr<UStaticMeshComponent> m_MeshComponent = nullptr;

	UPROPERTY();
	TSoftObjectPtr<UStaticMesh> m_Mesh = nullptr;
};
