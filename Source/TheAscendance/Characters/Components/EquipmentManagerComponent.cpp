// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentManagerComponent.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Items/HeldEquippableItem.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Characters/BaseCharacter.h"
#include "TheAscendance/Items/Structs/ItemData.h"
#include "TheAscendance/Game/Subsystems/ItemRegistrySubsystem.h"
#include "TheAscendance/Characters/Components/LoadoutComponent.h"
#include "TheAscendance/Abilities/Components/AbilityComponent.h"
#include "TheAscendance/Abilities/Enums/AbilitySlot.h"

// Sets default values for this component's properties
UEquipmentManagerComponent::UEquipmentManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UEquipmentManagerComponent::Init(ABaseCharacter* owner, UAbilityComponent* abilityComponent, ULoadoutComponent* loadoutComponent)
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

	m_LoadoutComponent = loadoutComponent;

	if (m_LoadoutComponent == nullptr)
	{
		LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] Loadout Component is invalid.");
		return;
	}

	m_LoadoutComponent->OnSpellsUpdate.AddUObject(this, &UEquipmentManagerComponent::UpdateAbilities);

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

		AHeldEquippableItem* newItem = world->SpawnActor<AHeldEquippableItem>(AHeldEquippableItem::StaticClass());

		if(newItem == nullptr)
		{
			LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] Failed to spawn HeldItem for part: %s", *UEnum::GetValueAsString(part));
			continue;
		}

		newItem->UnEquip();

		newItem->SetActorLocation(m_Owner->GetSocketLocationFromPart(part));
		newItem->K2_AttachToComponent(mesh, m_Owner->GetSocketNameFromPart(part), EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);

		m_Equipment.Add(part, newItem);
	}
}

bool UEquipmentManagerComponent::EquipItem(const FGameplayTag& itemTag, EEquippablePart part)
{
	if(m_LoadoutComponent == nullptr)
	{
		LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] Tried to Equip but Loadout Component is invalid.");
		return false;
	}

	if (itemTag.IsValid() == false)
	{
		LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] Tried to equip invalid ItemTag");
		return false;
	}

	UEquippableItemData* itemData = nullptr;

	if (UItemRegistrySubsystem* registry = UCoreFunctionLibrary::GetGameWorld()->GetGameInstance()->GetSubsystem<UItemRegistrySubsystem>())
	{
		itemData = registry->LoadItemData<UEquippableItemData>(itemTag);
	}

	if (itemData == nullptr)
	{
		LOG_WARNING("[EQUIPMENT MANAGER COMPONENT] ItemData was invalid when attempting to equip item: %s... Either the item doesn't exist, or is not of an equippable type", *itemTag.ToString());
		return false;
	}

	if (m_LoadoutComponent->IsPartEquipped(part) == true)
	{
		LOG_INFO("[EQUIPMENT MANAGER COMPONENT] Tried to equip item: %s to part: %s but it is already equipped.", *itemTag.ToString(), *UEnum::GetValueAsString(part));
		return false;
	}

	switch (itemData->EquipmentSlotUsed)
	{
		case EEquipmentSlot::ONE_HAND:
			if (part != EEquippablePart::RIGHT_HAND && part != EEquippablePart::LEFT_HAND)
			{
				LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] Tried to equip a ONE_HAND item to a non-hand part", *itemTag.ToString());
				return false;
			}
			break;
		case EEquipmentSlot::TWO_HAND:
			if (part != EEquippablePart::RIGHT_HAND && part != EEquippablePart::LEFT_HAND)
			{
				LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] Tried to equip a TWO_HAND item to a non-hand part", *itemTag.ToString());
				return false;
			}

			if (m_LoadoutComponent->IsPartEquipped(EEquippablePart::RIGHT_HAND) == false && m_LoadoutComponent->IsPartEquipped(EEquippablePart::LEFT_HAND) == false)
			{
				break;
			}
			else
			{
				LOG_INFO("[EQUIPMENT MANAGER COMPONENT] Tried to equip TWO_HAND item, but one or both hands were already equipped.");
				return false;
			}
	}

	if (m_Equipment.Contains(part) == false)
	{
		LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] EquipmentManager does not hold an item reference for: %s", *UEnum::GetValueAsString(part));
		return false;
	}

	TWeakObjectPtr<AHeldEquippableItem> heldItem = m_Equipment[part];

	if (heldItem == nullptr)
	{
		LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] EquipmentManager does not hold a reference to a valid Item Object for: %s", *UEnum::GetValueAsString(part));
		return false;
	}

	if (itemData->EquipmentSlotUsed == EEquipmentSlot::TWO_HAND)
	{			
		//LEFT OR RIGHT HAND already validated, other parts are not possible at this point
		part == EEquippablePart::RIGHT_HAND ? m_LoadoutComponent->BlockEquipItem(EEquippablePart::LEFT_HAND) : m_LoadoutComponent->BlockEquipItem(EEquippablePart::RIGHT_HAND);
	}

	m_LoadoutComponent->EquipItem(part, itemTag);

	heldItem->Init(Cast<UWeaponItemData>(itemData));
	UpdateAbilities();
	return true;
}

void UEquipmentManagerComponent::UnEquipItem(EEquippablePart part)
{
	if (m_LoadoutComponent == nullptr)
	{
		LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] Tried to UnEquip but Loadout Component is invalid.");
		return;
	}

	if (m_Equipment.Contains(part) == false)
	{
		LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] EquipmentManager does not hold an item reference for: %s", *UEnum::GetValueAsString(part));
		return;
	}

	AHeldEquippableItem* heldItem = m_Equipment[part];

	if (heldItem == nullptr)
	{
		LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] EquipmentManager does not hold a reference to a valid Item Object for: %s", *UEnum::GetValueAsString(part));
		return;
	}

	const UEquippableItemData* itemData = heldItem->GetItemData();

	if (itemData == nullptr)
	{
		LOG_WARNING("[EQUIPMENT MANAGER COMPONENT] ItemData was invalid when attempting to unequip part: %s... Either the part has no equipped item, or the item contains invalid data (logged internally)", *UEnum::GetValueAsString(part));
		return;
	}

	if (itemData->EquipmentSlotUsed == EEquipmentSlot::TWO_HAND)
	{
		//LEFT OR RIGHT HAND already validated, other parts are not possible at this point
		part == EEquippablePart::RIGHT_HAND ? m_LoadoutComponent->UnEquipItem(EEquippablePart::LEFT_HAND) : m_LoadoutComponent->UnEquipItem(EEquippablePart::RIGHT_HAND);
	}

	m_LoadoutComponent->UnEquipItem(part);
	heldItem->UnEquip();
	UpdateAbilities();
}

EIdleType UEquipmentManagerComponent::GetIdleType()
{
	EIdleType type = EIdleType::NO_ITEMS;

	if (AHeldEquippableItem* heldItem = m_Equipment[EEquippablePart::LEFT_HAND])
	{
		if (const UEquippableItemData* itemData = heldItem->GetItemData())
		{
			if (itemData->EquipmentSlotUsed == EEquipmentSlot::TWO_HAND)
			{
				return EIdleType::EQUIPPED_TWOHANDED;
			}

			type = EIdleType::EQUIPPED_OFFHAND;
		}
	}
	else
	{
		LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] EquipmentManager does not hold a reference to a valid Item Object for: %s", *UEnum::GetValueAsString(EEquippablePart::LEFT_HAND));
	}

	if (AHeldEquippableItem* heldItem = m_Equipment[EEquippablePart::RIGHT_HAND])
	{
		if (const UEquippableItemData* itemData = heldItem->GetItemData())
		{
			if (itemData->EquipmentSlotUsed == EEquipmentSlot::TWO_HAND)
			{
				return EIdleType::EQUIPPED_TWOHANDED;
			}

			if(type == EIdleType::EQUIPPED_OFFHAND)
			{
				type = EIdleType::EQUIPPED_BOTHHANDS;
			}
			else
			{
				type = EIdleType::EQUIPPED_MAINHAND;
			}
		}
	}
	else
	{
		LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] EquipmentManager does not hold a reference to a valid Item Object for: %s", *UEnum::GetValueAsString(EEquippablePart::RIGHT_HAND));
	}

	return type;
}

bool UEquipmentManagerComponent::IsHoldingTwoHandedItem()
{
	if (AHeldEquippableItem* heldItem = m_Equipment[EEquippablePart::LEFT_HAND])
	{
		if (const UEquippableItemData* itemData = heldItem->GetItemData())
		{
			if (itemData->EquipmentSlotUsed == EEquipmentSlot::TWO_HAND)
			{
				return true;
			}
		}
	}
	else
	{
		LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] EquipmentManager does not hold a reference to a valid Item Object for: %s", *UEnum::GetValueAsString(EEquippablePart::LEFT_HAND));
	}

	if (AHeldEquippableItem* heldItem = m_Equipment[EEquippablePart::RIGHT_HAND])
	{
		if (const UEquippableItemData* itemData = heldItem->GetItemData())
		{
			if (itemData->EquipmentSlotUsed == EEquipmentSlot::TWO_HAND)
			{
				return true;
			}
		}
	}
	else
	{
		LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] EquipmentManager does not hold a reference to a valid Item Object for: %s", *UEnum::GetValueAsString(EEquippablePart::RIGHT_HAND));
	}

	return false;
}

void UEquipmentManagerComponent::UpdateAbilities()
{
	if(m_AbilityComponent == nullptr)
	{
		LOG_ERROR("[EQUIPMENT MANAGER COMPONENT] Tried to Update Abilities but Ability Component is invalid.");
		return;
	}

	TMap<EAbilitySlot, FGameplayTag> abilities = m_LoadoutComponent->GetSpellsCopy();

	if (AHeldEquippableItem* heldItem = m_Equipment[EEquippablePart::RIGHT_HAND])
	{
		if (const UWeaponItemData* itemData = heldItem->GetItemData())
		{
			if (itemData->PrimaryAbility.DoesSpellOverride == false)
			{
				abilities.FindOrAdd(EAbilitySlot::MAINHAND_PRIMARY) = itemData->PrimaryAbility.AbilityTag;
			}

			if (itemData->AltAbility.DoesSpellOverride == false)
			{
				abilities.FindOrAdd(EAbilitySlot::MAINHAND_ALT) = itemData->AltAbility.AbilityTag;
			}
		}
	}

	if (AHeldEquippableItem* heldItem = m_Equipment[EEquippablePart::LEFT_HAND])
	{
		if(const UWeaponItemData* itemData = heldItem->GetItemData())
		{
			if (itemData->PrimaryAbility.DoesSpellOverride == false)
			{
				abilities.FindOrAdd(EAbilitySlot::OFFHAND_PRIMARY) = itemData->PrimaryAbility.AbilityTag;
			}

			if (itemData->AltAbility.DoesSpellOverride == false)
			{
				abilities.FindOrAdd(EAbilitySlot::OFFHAND_ALT) = itemData->AltAbility.AbilityTag;
			}
		}
	}

	m_AbilityComponent->SetAbilities(abilities);
}

// Called when the game starts
void UEquipmentManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

