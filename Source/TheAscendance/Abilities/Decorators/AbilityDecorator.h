// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TheAscendance/Abilities/Interfaces/Ability.h"
#include "AbilityDecorator.generated.h"

class IAbility;

UCLASS(Abstract)
class THEASCENDANCE_API UAbilityDecorator : public UObject, public IAbility
{
	GENERATED_BODY()
	
public:
	void Decorate(IAbility* decorator);
	virtual void SetDecoratedSelf(IAbility* decoratedSelf) override;

	virtual void Start() override;
	virtual void Stop() override;
	virtual void Execute() override;

	virtual const FGameplayTag& GetAbilityTag() const override;
	virtual bool CanStart() const override;

	virtual void OnInputReleased() override;

	virtual float PlayAnimMontageOnOwner(UAnimMontage* animation) override;

protected:
	UPROPERTY()
	TScriptInterface<IAbility> m_DecoratedAbility = nullptr;
};
