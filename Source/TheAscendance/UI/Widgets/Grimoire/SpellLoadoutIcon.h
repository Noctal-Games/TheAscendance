// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "SpellLoadoutIcon.generated.h"

class UGrimoire;
class USpellDataEntryObject;
class UImage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHovered);

UCLASS()
class THEASCENDANCE_API USpellLoadoutIcon : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public: 
	void Init(USpellDataEntryObject* spellDataEntry);


	virtual FReply NativeOnFocusReceived(const FGeometry& geometryEvent, const FFocusEvent& focusEvent) override;
	virtual void NativeOnMouseEnter(const FGeometry& geometryEvent, const FPointerEvent& pointerEvent) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& geometryEvent, const FPointerEvent& pointerEvent) override;

	virtual void NativePreConstruct() override;
private:
	UFUNCTION()
	void UpdateGrimoireSpellInfo();

private:
	UPROPERTY()
	TObjectPtr<USpellDataEntryObject> m_SpellDataEntry = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UImage> m_SpellIcon = nullptr;

	UPROPERTY()
	TWeakObjectPtr<UGrimoire> m_Grimoire = nullptr;
};
