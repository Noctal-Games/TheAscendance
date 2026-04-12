// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "Ability.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAbility : public UInterface
{
	GENERATED_BODY()
};

class UAbilityData;
class UAbilityComponent;

class THEASCENDANCE_API IAbility
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Init(UAbilityComponent* ownerComponent, UAbilityData* abilityData) {};

	virtual void Start() {};
	virtual void Stop() {};
	virtual void Execute() {};

	virtual const FGameplayTag& GetAbilityTag() const { return FGameplayTag::EmptyTag; }
	virtual bool CanStart() const { return true; }
};
