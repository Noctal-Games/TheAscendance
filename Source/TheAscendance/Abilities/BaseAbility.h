// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/Ability.h"
#include "BaseAbility.generated.h"

class UAbilityData;
class UAbilityComponent;

UCLASS()
class THEASCENDANCE_API UBaseAbility : public UObject, public IAbility
{
	GENERATED_BODY()
	
public:
	virtual void Init(UAbilityComponent* ownerComponent, UAbilityData* abilityData);
	
	virtual void Activate();
	virtual void End();

	virtual void OnHitWindowStart();
	virtual void OnHitWindowEnd();

	virtual const FGameplayTag& GetAbilityTag() const override;

protected:
	UPROPERTY()
	TObjectPtr<UAbilityData> m_AbilityData = nullptr;

private:
	UPROPERTY()
	TWeakObjectPtr<UAbilityComponent> m_OwnerComponent = nullptr;
};
