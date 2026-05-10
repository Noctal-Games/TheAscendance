// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TheAscendance/Abilities/Enums/AbilitySlot.h"
#include "TheAscendance/Characters/Enums/CharacterStat.h"
#include "TheAscendance/Abilities/Structs/AbilityInfo.h"
#include "GameplayTagContainer.h"
// Test
#include "TheAscendance/Abilities/AbilityFactory.h"
//
#include "AbilityComponent.generated.h"

class IAbility;
class ABaseCharacter;
class UAnimMontage;
class UAbilityData;
class APlayableGameMode;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnAbilitiesUpdate, const TArray<FAbilityInfo>&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAbilityCooldown, const FGameplayTag& /*abilityTag*/, float /*remaining*/, float /*max*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEASCENDANCE_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilityComponent();

	void SetAbilities(const TMap<EAbilitySlot, FGameplayTag>& abilityTags);

	//Starts the overall ability. Charging, animations, etc.
	void StartAbility(EAbilitySlot slot);
	//Triggered by animations, this is the actual ability start event. So when a spell is cast, a melee collision is activated, etc.
	void TriggerAbility();
	//Stops the ability. Clears up all timer handles, etc.
	void StopAbility();

	void OnInputReleased();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FVector GetCastLocation();
	FVector GetCastForward();

	void AffectOwnerStat(ECharacterStat stat, int amount);
	float GetOwnerStat(ECharacterStat stat);

protected:
	friend class UBaseAbility;
	// Called when the game starts
	virtual void BeginPlay() override;

	float PlayAnimMontageOnOwner(UAnimMontage* animation);

private:
	void TriggerOnAbilitiesUpdate();
	void ProcessAbilityPair(TMap<EAbilitySlot, UAbilityData*>& abilityData, EAbilitySlot mainSlot, EAbilitySlot offSlot, const FString& label, APlayableGameMode* gameMode);

public:
	FOnAbilitiesUpdate OnAbilitiesUpdate;
	FOnAbilityCooldown OnAbilityCooldown;
	 
private:
	UPROPERTY()
	TWeakObjectPtr<ABaseCharacter> m_Owner = nullptr;

	UPROPERTY()
	TArray<TScriptInterface<IAbility>> m_Abilities;

	FTimerHandle m_AbilityTimerHandle;

	UPROPERTY()
	TScriptInterface<IAbility> m_ActiveAbility = nullptr;

	TUniquePtr<AbilityFactory> m_Factory = nullptr;
};
