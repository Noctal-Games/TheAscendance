// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TheAscendance/Characters/Enums/EquippablePart.h"
#include "TheAscendance/Characters/Structs/LoadoutSlotData.h"
#include "GameplayTagContainer.h"
#include "TheAscendance/Abilities/Enums/AbilitySlot.h"
#include "LoadoutComponent.generated.h"

USTRUCT()
struct FEquipmentMap
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<EEquippablePart, FGameplayTag> Map;
};

class ABaseCharacter;

DECLARE_MULTICAST_DELEGATE(FOnSpellsUpdate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEASCENDANCE_API ULoadoutComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULoadoutComponent();

	void EquipItem(EEquippablePart part, const FGameplayTag& itemTag);
	void BlockEquipItem(EEquippablePart part);
	void UnEquipItem(EEquippablePart part);

	void SetSpells(const TMap<EAbilitySlot, FGameplayTag>& spellTags);

	bool IsPartEquipped(const EEquippablePart& part);
protected:
	friend class UGrimoire;
	friend class UEquipmentManagerComponent;
	const TMap<EAbilitySlot, FGameplayTag>& GetSpellTags() const;
	TMap<EAbilitySlot, FGameplayTag> GetSpellsCopy() const;

private:
	bool Contains(EEquippablePart part);

protected:
	FOnSpellsUpdate OnSpellsUpdate;

private:
	TArray<FLoadoutSlotData> m_Loadout;
	TMap<EAbilitySlot, FGameplayTag> m_SpellTags;
};
