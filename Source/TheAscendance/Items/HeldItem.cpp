// Fill out your copyright notice in the Description page of Project Settings.

#include "HeldItem.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/StreamableFunctionLibrary.h"
#include "TheAscendance/Characters/BaseCharacter.h"
#include "TheAscendance/Items/Structs/ItemData.h"
#include "TheAscendance/Items/Structs/WeaponData.h"
#include "TheAscendance/Items/Enums/WeaponType.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

AHeldItem::AHeldItem()
{
	m_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	checkf(m_MeshComponent, TEXT("Item failed to initialise StaticMeshComponent"));
	m_MeshComponent->SetSimulatePhysics(false);
	m_MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshComponent->SetEnableGravity(false);
	m_MeshComponent->SetSimulatePhysics(false);
}

void AHeldItem::Init(UWeaponItemData* itemData)
{
	if (itemData == nullptr)
	{
		LOG_ERROR("[HELD ITEM] Tried to initialise HeldItem with invalid ItemData");
		return;
	}

	m_ItemData = itemData;

	m_Mesh = m_ItemData->ItemMesh;

	if (m_Mesh.IsNull() == false)
	{
		UStreamableFunctionLibrary::RequestAsyncLoad(m_Mesh.ToSoftObjectPath(), [this]() { SetStaticMesh(); });
	}
}

void AHeldItem::UnEquip()
{
	m_ItemData = nullptr;
	m_Mesh.Reset();
	m_MeshComponent->SetStaticMesh(nullptr);
}

void AHeldItem::SetStaticMesh()
{
	if (m_Mesh.Get() != nullptr)
	{
		m_MeshComponent->SetStaticMesh(m_Mesh.Get());
	}
}