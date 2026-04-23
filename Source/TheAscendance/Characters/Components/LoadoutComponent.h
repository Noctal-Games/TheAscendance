// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TheAscendance/Characters/Enums/EquippablePart.h"
#include "TheAscendance/Characters/Structs/LoadoutSlotData.h"
#include "GameplayTagContainer.h"
#include "LoadoutComponent.generated.h"

USTRUCT()
struct FEquipmentMap
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<EEquippablePart, FGameplayTag> Map;
};

class ABaseCharacter;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnUpdate, const TArray<FLoadoutSlotData>& /*equipmentData*/, const TArray<FGameplayTag>& /*spellTags*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEASCENDANCE_API ULoadoutComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULoadoutComponent();

	void EquipItem(EEquippablePart part, const FGameplayTag& itemTag);
	void UnEquipItem(EEquippablePart part);

	void SetSpells(const TArray<FGameplayTag>& spellTags);
protected:
	friend class UGrimoire;
	const TArray<FGameplayTag>& GetSpellTags() const;

private:
	bool Contains(EEquippablePart part);

public:
	FOnUpdate OnUpdate;

private:
	TArray<FLoadoutSlotData> m_Loadout;
	TArray<FGameplayTag> m_SpellTags;
};
