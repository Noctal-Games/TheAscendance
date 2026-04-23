// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentManagerComponent.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Items/HeldItem.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Characters/BaseCharacter.h"
#include "TheAscendance/Items/Structs/ItemData.h"

// Sets default values for this component's properties
UEquipmentManagerComponent::UEquipmentManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UEquipmentManagerComponent::Init(ABaseCharacter* owner, UAbilityComponent* abilityComponent)
{
	m_Owner = owner;

	if (m_Owner == nullptr)
	{
		LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] Owner is invalid.");
		return;
	}

	m_AbilityComponent = abilityComponent;

	if (m_AbilityComponent == nullptr)
	{
		LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] Ability Component is invalid.");
		return;
	}

	UWorld* world = UCoreFunctionLibrary::GetGameWorld();
	
	if (world == nullptr)
	{
		LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] World is invalid.");
		return;
	}


	USkeletalMeshComponent* mesh = m_Owner->GetEquipmentMesh();

	if (mesh == nullptr)
	{
		LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] Owner's mesh is invalid.");
		return;
	}

	for (const EEquippablePart& part : TEnumRange<EEquippablePart>())
	{
		if (part == EEquippablePart::NONE)
		{
			continue;
		}

		AHeldItem* newItem = world->SpawnActor<AHeldItem>(AHeldItem::StaticClass());

		if(newItem == nullptr)
		{
			LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] Failed to spawn HeldItem for part: %s", *UEnum::GetValueAsString(part));
			continue;
		}

		newItem->SetItemOwner(m_Owner.Get());
		newItem->UnEquip();

		newItem->SetActorLocation(m_Owner->GetSocketLocationFromPart(part));
		newItem->K2_AttachToComponent(mesh, m_Owner->GetSocketNameFromPart(part), EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);

		m_Equipment.Add(part, newItem);
	}
}

void UEquipmentManagerComponent::EquipItem(const FGameplayTag& itemTag)
{
}

void UEquipmentManagerComponent::UnEquipItem(EEquippablePart part)
{
}

void UEquipmentManagerComponent::OnLoadoutUpdated(const TArray<FLoadoutSlotData>& equipmentData, const TArray<FGameplayTag>& spellTags)
{
	for(const auto& data : equipmentData)
	{
		//EquipPart should never be NONE as the LoadoutComponent should filter these out
		if (m_Equipment.Contains(data.EquippedPart) == false)
		{
			LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] EquipmentManager does not hold a reference for: %s", *UEnum::GetValueAsString(data.EquippedPart));
			continue;
		}

		TWeakObjectPtr<AHeldItem> heldItem = m_Equipment[data.EquippedPart];

		if (heldItem == nullptr)
		{
			LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] EquipmentManager does not hold a reference to a valid Item Object for: %s", *UEnum::GetValueAsString(data.EquippedPart));
			continue;
		}

		if (data.ItemTag.IsValid() == false)
		{
			heldItem->UnEquip();
			continue;
		}

		FItemData* itemData = nullptr;

		if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
		{
			itemData = gameMode->GetItemData(data.ItemTag);
		}

		if (itemData == nullptr)
		{
			LOG_WARNING("[EQUIPMENT MANAGER COMPONENT] ItemData was invalid when attempting to equip item: %s... Unequipping the target part entirely as a failsafe.", *data.ItemTag.ToString())
			heldItem->UnEquip();
		}
		else
		{
			heldItem->Init(itemData);
		}

		// Get Item Weapon Data if valid and determine active ability, then provide ability tag to component
	}
}

// Called when the game starts
void UEquipmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

