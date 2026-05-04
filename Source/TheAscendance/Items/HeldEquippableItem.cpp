// Fill out your copyright notice in the Description page of Project Settings.


#include "HeldEquippableItem.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/StreamableFunctionLibrary.h"
#include "TheAscendance/Items/Structs/ItemData.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AHeldEquippableItem::AHeldEquippableItem()
{
	m_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	checkf(m_MeshComponent, TEXT("Item failed to initialise StaticMeshComponent"));
	m_MeshComponent->SetSimulatePhysics(false);
	m_MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshComponent->SetEnableGravity(false);
	m_MeshComponent->SetSimulatePhysics(false);
}

void AHeldEquippableItem::Init(UWeaponItemData* itemData)
{
	if (itemData == nullptr)
	{
		LOG_ERROR("[HELD EQUIPPABLE ITEM] Tried to initialise HeldItem with invalid ItemData");
		return;
	}

	m_ItemData = itemData;

	m_Mesh = m_ItemData->ItemMesh;

	if (m_Mesh.IsNull() == false)
	{
		TWeakObjectPtr<AHeldEquippableItem> weakThis(this);
		UStreamableFunctionLibrary::RequestAsyncLoad(m_Mesh.ToSoftObjectPath(), [weakThis]() { if (weakThis.IsValid()) { weakThis->SetStaticMesh(); }});
	}
	else
	{
		LOG_ERROR("[HELD EQUIPPABLE ITEM] Item Mesh not set");
	}
}

void AHeldEquippableItem::UnEquip()
{
	m_ItemData = nullptr;
	m_Mesh.Reset();
	m_MeshComponent->SetStaticMesh(nullptr);
}

const UWeaponItemData* AHeldEquippableItem::GetItemData() const
{
	return m_ItemData;
}

void AHeldEquippableItem::SetStaticMesh()
{
	if (m_Mesh.Get() != nullptr)
	{
		m_MeshComponent->SetStaticMesh(m_Mesh.Get());
	}
	else
	{
		LOG_ERROR("[HELD EQUIPPABLE ITEM] Item Mesh failed to load");
	}
}
