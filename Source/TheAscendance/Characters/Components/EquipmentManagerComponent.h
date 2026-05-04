// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TheAscendance/Characters/Structs/LoadoutSlotData.h"
#include "TheAscendance/Characters/Enums/EquippablePart.h"
#include "GameplayTagContainer.h"
#include "TheAscendance/Abilities/Structs/AbilityData.h"
#include "EquipmentManagerComponent.generated.h"

class UAbilityComponent;
class ULoadoutComponent;
class ABaseCharacter;
class AHeldEquippableItem;
class APlayableGameMode;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class THEASCENDANCE_API UEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentManagerComponent();

	void Init(ABaseCharacter* owner, UAbilityComponent* abilityComponent, ULoadoutComponent* loadoutComponent);

	bool EquipItem(const FGameplayTag& itemTag, EEquippablePart part);
	void UnEquipItem(EEquippablePart part);

protected:
	void UpdateAbilities();

	// Called when the game starts
	virtual void BeginPlay() override;

private:
	TArray<FProcessedAbility> ProcessAbilities();
	void ProcessAbilityPair(APlayableGameMode* gameMode, TArray<FProcessedAbility>& inAbilities, TMap<EAbilitySlot, FGameplayTag>& abilityData, EAbilitySlot mainSlot, EAbilitySlot offSlot, const FString& label);

private:
	UPROPERTY()
	TMap<EEquippablePart, TObjectPtr<AHeldEquippableItem>> m_Equipment;

	UPROPERTY()
	TWeakObjectPtr<ABaseCharacter> m_Owner = nullptr;

	UPROPERTY()
	TWeakObjectPtr<UAbilityComponent> m_AbilityComponent = nullptr;
	UPROPERTY()
	TWeakObjectPtr<ULoadoutComponent> m_LoadoutComponent = nullptr;
};
