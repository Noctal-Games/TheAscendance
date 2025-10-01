// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Structs/ItemData.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	checkf(m_MeshComponent, TEXT("Item failed to initialise StaticMeshComponent"));
	m_MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_MeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
	m_MeshComponent->SetEnableGravity(true);
	m_MeshComponent->SetSimulatePhysics(true);
}

void AItem::Init(FItemData* itemData)
{
	if (itemData == nullptr)
	{
		LOG_ERROR("Tried to initialise Item with null ItemData");
		return;
	}

	m_ItemData = MakeShared<FItemData>(*itemData);

	m_Mesh = m_ItemData->ItemMesh;

	if (m_Mesh.IsNull() == false)
	{
		UCoreFunctionLibrary::RequestAsyncLoad(m_Mesh.ToSoftObjectPath(), [this]() { SetStaticMesh(); });
	}
}

void AItem::SetStaticMesh()
{
	if (m_Mesh.Get() != nullptr)
	{
		m_MeshComponent->SetStaticMesh(m_Mesh.Get());
	}
}
