// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Susceptible.generated.h"

// This class does not need to be modified.
UINTERFACE(NotBlueprintable, BlueprintType)
class USusceptible : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class THEASCENDANCE_API ISusceptible
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, Category = "Susceptible Interface")
	virtual void Heal(int amount) {};
	UFUNCTION(BlueprintCallable, Category = "Susceptible Interface")
	virtual void Damage(int amount) {};
	UFUNCTION(BlueprintCallable, Category = "Susceptible Interface")
	virtual void ReduceStamina(int amount) {};
	UFUNCTION(BlueprintCallable, Category = "Susceptible Interface")
	virtual int GetStat(ECharacterStat stat) { return 0; }

	UFUNCTION(BlueprintCallable, Category = "Susceptible Interface")
	virtual bool IsDead() { return true; };
};
