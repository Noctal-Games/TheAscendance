// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TheAscendance/Characters/Structs/LoadoutSlotData.h"
#include "TheAscendance/Characters/Enums/EquippablePart.h"
#include "GameplayTagContainer.h"
#include "EquipmentManagerComponent.generated.h"

class UAbilityComponent;
class ABaseCharacter;
class AHeldItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEASCENDANCE_API UEquipmentManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentManagerComponent();

	void Init(ABaseCharacter* owner, UAbilityComponent* abilityComponent);

	void EquipItem(const FGameplayTag& itemTag);
	void UnEquipItem(EEquippablePart part);

	void OnLoadoutUpdated(const TArray<FLoadoutSlotData>& equipmentData, const TArray<FGameplayTag>& spellTags);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TMap<EEquippablePart, TObjectPtr<AHeldItem>> m_Equipment;

	UPROPERTY()
	TWeakObjectPtr<ABaseCharacter> m_Owner = nullptr;

	UPROPERTY()
	TWeakObjectPtr<UAbilityComponent> m_AbilityComponent = nullptr;
};
