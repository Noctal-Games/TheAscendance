// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TheAscendance/Items/Enums/WeaponType.h"
#include "HeldItem.generated.h"

class ABaseCharacter;
class UBoxComponent;
class UStaticMeshComponent;
class UStaticMesh;
struct FItemData;
struct FWeaponData;
struct FWeaponTypeData;

UCLASS()
class THEASCENDANCE_API AHeldItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AHeldItem();

	void SetItemOwner(ABaseCharacter* owner);

	void Init(FItemData* itemData);
	void SetStaticMesh();
	void UnEquip();

	bool StartPrimaryAttack();
	bool StartSecondaryAttack();

	void EndAttack();

	EWeaponType GetWeaponType();

	//void UpdateItem();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void EndPrimaryAttack();
	void EndSecondaryAttack();

protected:
	friend class ABaseCharacter;

	bool m_IsOffHand = false;
private:
	TWeakObjectPtr<ABaseCharacter> m_Owner = nullptr;

	UPROPERTY();
	TSoftObjectPtr<UStaticMesh> m_Mesh = nullptr;

	UPROPERTY();
	TObjectPtr<UStaticMeshComponent> m_MeshComponent = nullptr;

	TSharedPtr<FItemData> m_ItemData = nullptr;
	UPROPERTY()
	TObjectPtr<UBoxComponent> m_Collider = nullptr;

	UPROPERTY()
	TArray<TObjectPtr<ABaseCharacter>> m_HitCharacters;

	TSharedPtr<FWeaponData> m_WeaponData = nullptr;
	TSharedPtr<FWeaponTypeData> m_WeaponTypeData = nullptr;

	bool m_IsPrimaryAttacking = false;
};
