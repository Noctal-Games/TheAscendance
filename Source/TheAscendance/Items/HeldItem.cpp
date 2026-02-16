// Fill out your copyright notice in the Description page of Project Settings.

#include "HeldItem.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
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
	m_MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_MeshComponent->SetEnableGravity(false);
	m_MeshComponent->SetSimulatePhysics(false);

	m_Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	checkf(m_Collider, TEXT("Combat Item Collider failed to initialise"));
	m_Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_Collider->SetVisibility(false);
	m_Collider->SetHiddenInGame(true);
	m_Collider->SetupAttachment(m_MeshComponent);

	SetRootComponent(m_MeshComponent);
}

void AHeldItem::SetItemOwner(ABaseCharacter* owner)
{
	m_Owner = owner;
}

void AHeldItem::Init(FItemData* itemData)
{
	if (itemData == nullptr)
	{
		LOG_ERROR("Tried to initialise HeldItem with null ItemData");
		return;
	}

	m_ItemData = MakeShared<FItemData>(*itemData);

	m_Mesh = m_ItemData->ItemMesh;

	if (m_Mesh.IsNull() == false)
	{
		UCoreFunctionLibrary::RequestAsyncLoad(m_Mesh.ToSoftObjectPath(), [this]() { SetStaticMesh(); });
	}

	if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
	{
		if (FWeaponData* weaponData = gameMode->GetWeaponData(m_ItemData->ItemTag))
		{
			m_WeaponData = MakeShared<FWeaponData>(*weaponData);
		}
		else
		{
			LOG_ERROR("Tried to initialise HeldItem with invalid WeaponData");
			return;
		}

		if (const FWeaponTypeData* typeData = gameMode->GetWeaponTypeData(m_WeaponData->WeaponType))
		{
			m_WeaponTypeData = MakeShared<FWeaponTypeData>(*typeData);
		}
		else
		{
			LOG_ERROR("Tried to initialise HeldItem with invalid WeaponTypeData");
			return;
		}
	}
	else
	{
		LOG_ERROR("Tried to initialise HeldItem within invalid GameMode");
		return;
	}
}

void AHeldItem::SetStaticMesh()
{
	if (m_Mesh.Get() != nullptr)
	{
		m_MeshComponent->SetStaticMesh(m_Mesh.Get());

		FBoxSphereBounds3d bounds = m_Mesh->GetBounds();
		bounds.BoxExtent.Y += bounds.BoxExtent.Y * 0.25f;
		bounds.BoxExtent.Z += bounds.BoxExtent.Z * 0.25f;

		m_Collider->SetBoxExtent(bounds.BoxExtent, true);
	}
}

void AHeldItem::UnEquip()
{
	m_ItemData.Reset();
	m_Mesh.Reset();
	m_MeshComponent->SetStaticMesh(nullptr);

	m_WeaponData.Reset();
	m_WeaponTypeData.Reset();

	if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
	{
		if (const FWeaponTypeData* typeData = gameMode->GetWeaponTypeData(EWeaponType::HAND))
		{
			m_WeaponTypeData = MakeShared<FWeaponTypeData>(*typeData);
		}
		else
		{
			LOG_ERROR("Tried to UnEquip HeldItem with invalid WeaponTypeData");
			return;
		}
	}
	else
	{
		LOG_ERROR("Tried to UnEquip HeldItem within invalid GameMode");
		return;
	}
}

bool AHeldItem::StartPrimaryAttack()
{
	if (m_Owner == nullptr || m_WeaponTypeData == nullptr)
	{
		return false;
	}

	if (m_WeaponTypeData->CastsSpell == false)
	{
		LOG_ONSCREEN(-1, 0.5, FColor::Green, "Start Melee Attack: %s", *(m_ItemData != nullptr ? m_ItemData->ItemName.ToString() : FString("HAND")));
	}
	else
	{
		LOG_ONSCREEN(-1, 0.5, FColor::Green, "Start Spell Attack: %s", *(m_ItemData != nullptr ? m_ItemData->ItemName.ToString() : FString("HAND")));
	}
	//If Melee
	//Turn on collider
	//Else Spell
	//Start spell animation

	m_IsPrimaryAttacking = true;
	return true;
}

bool AHeldItem::StartSecondaryAttack()
{
	if (m_Owner == nullptr || m_WeaponTypeData == nullptr)
	{
		return false;
	}

	if (m_WeaponTypeData->CastsSpell == false)
	{
		LOG_ONSCREEN(-1, 0.5, FColor::Green, "Start Melee Attack: %s", *(m_ItemData != nullptr ? m_ItemData->ItemName.ToString() : FString("HAND")));
	}
	else
	{
		LOG_ONSCREEN(-1, 0.5, FColor::Green, "Start Spell Attack: %s", *(m_ItemData != nullptr ? m_ItemData->ItemName.ToString() : FString("HAND")));
	}

	//If Melee
	//Turn on collider
	//Else Spell
	//Start spell animation

	m_IsPrimaryAttacking = false;
	return true;
}

void AHeldItem::EndAttack()
{
	m_HitCharacters.Empty();
	m_IsPrimaryAttacking ? EndPrimaryAttack() : EndSecondaryAttack();
}

EWeaponType AHeldItem::GetWeaponType()
{
	if (m_WeaponData == nullptr)
	{
		return EWeaponType::HAND;
	}

	return m_WeaponData->WeaponType;
}

// Called when the game starts or when spawned
void AHeldItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHeldItem::EndPrimaryAttack()
{
	if (m_WeaponTypeData->CastsSpell == false)
	{
		LOG_ONSCREEN(-1, 0.5, FColor::Green, "End Melee Attack: %s", *(m_ItemData != nullptr ? m_ItemData->ItemName.ToString() : FString("HAND")));
	}
	else
	{
		LOG_ONSCREEN(-1, 0.5, FColor::Green, "End Spell Attack: %s", *(m_ItemData != nullptr ? m_ItemData->ItemName.ToString() : FString("HAND")));
		m_Owner->GetCastStartFunc.BindLambda([this]() { return GetActorLocation(); });
		m_Owner->CastSpell(m_IsOffHand == false ? 1 : 3);
	}
}

void AHeldItem::EndSecondaryAttack()
{
	if (m_WeaponTypeData->CastsSpell == false)
	{
		LOG_ONSCREEN(-1, 0.5, FColor::Green, "End Melee Attack: %s", *(m_ItemData != nullptr ? m_ItemData->ItemName.ToString() : FString("HAND")));
	}
	else
	{
		LOG_ONSCREEN(-1, 0.5, FColor::Green, "End Spell Attack: %s", *(m_ItemData != nullptr ? m_ItemData->ItemName.ToString() : FString("HAND")));
		m_Owner->GetCastStartFunc.BindLambda([this]() { return GetActorLocation(); });
		m_Owner->CastSpell(m_IsOffHand == false ? 2 : 4);
	}
}
