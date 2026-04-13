// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/Ability.h"
#include "BaseAbility.generated.h"

class UAbilityData;
class UAbilityComponent;
class UAnimMontage;

UCLASS()
class THEASCENDANCE_API UBaseAbility : public UObject, public IAbility
{
	GENERATED_BODY()
	
public:
	virtual void Init(UAbilityComponent* ownerComponent, UAbilityData* abilityData) override;
	
	virtual void Start() override;
	virtual void Stop() override;
	virtual void Execute() override;

	virtual void OnInputReleased() override;

	virtual const FGameplayTag& GetAbilityTag() const override;

	virtual float PlayAnimMontageOnOwner(UAnimMontage* animation) override;

protected:
	UPROPERTY()
	TObjectPtr<UAbilityData> m_AbilityData = nullptr;

private:
	UPROPERTY()
	TWeakObjectPtr<UAbilityComponent> m_OwnerComponent = nullptr;

	UPROPERTY()
	TSoftObjectPtr<UAnimMontage> m_AbilityAnimation = nullptr;

	FTimerHandle m_AbilityDurationHandle;
};
