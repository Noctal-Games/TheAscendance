// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "ActionBar.generated.h"

class ULoadoutComponent;
class UActionBarIcon;
class UAbilityComponent;
struct FAbilityInfo;
struct FEquipmentMap;

UCLASS()
class THEASCENDANCE_API UActionBar : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(UAbilityComponent* abilityComponent);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
protected:
	void OnAbilitiesUpdated(const TArray<FAbilityInfo>& abilityInfo);

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget, DisplayName = "OffHand Action Slot 1"))
	TObjectPtr<UActionBarIcon> m_OffHandActionSlot1 = nullptr;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget, DisplayName = "OffHand Action Slot 2"))
	TObjectPtr<UActionBarIcon> m_OffHandActionSlot2 = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget, DisplayName = "MainHand Action Slot 1"))
	TObjectPtr<UActionBarIcon> m_MainHandActionSlot1 = nullptr;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget, DisplayName = "MainHand Action Slot 2"))
	TObjectPtr<UActionBarIcon> m_MainHandActionSlot2 = nullptr;
};
