// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
// Test
#include "TheAscendance/Abilities/AbilityFactory.h"
//
#include "AbilityComponent.generated.h"

class IAbility;
class ABaseCharacter;
class UAnimMontage;
class UAbilityData;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THEASCENDANCE_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilityComponent();

	virtual void SetAbilities(const TArray<FGameplayTag>& abilityTags);
	void TestSetAbilities(const TArray<TObjectPtr<UAbilityData>>& abilities);

	virtual void StartAbility(int slot);
	virtual void StopAbility();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	friend class UBaseAbility;
	// Called when the game starts
	virtual void BeginPlay() override;

	float PlayAnimMontageOnOwner(UAnimMontage* animation);
public:
	static constexpr int MaxAbilities = 4;

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
