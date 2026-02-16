// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TheAscendance/Characters/Enums/EquippablePart.h"
#include "TheAscendance/Characters/Structs/LoadoutSlotData.h"
#include "GameplayTagContainer.h"
#include "LoadoutComponent.generated.h"

class ABaseCharacter;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSpellsUpdated, const TArray<FGameplayTag>& /*SpellTags*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEASCENDANCE_API ULoadoutComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULoadoutComponent();

	void EquipItem(EEquippablePart part, int itemID);
	void UnEquipItem(EEquippablePart part);

	void SetSpells(const TArray<FGameplayTag>& spellTags);
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	bool Contains(EEquippablePart part);

public:
	FOnSpellsUpdated OnSpellsUpdated;

private:
	TWeakObjectPtr<ABaseCharacter> m_Owner = nullptr;

	TArray<TSharedPtr<FLoadoutSlotData>> m_Loadout;
	TArray<FGameplayTag> m_SpellTags;
};
