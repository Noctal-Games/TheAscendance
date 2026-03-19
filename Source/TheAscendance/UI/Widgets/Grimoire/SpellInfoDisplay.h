// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "SpellInfoDisplay.generated.h"

class USpellDataEntryObject;
class USpellData;
class UTextBlock;
class UImage;

UCLASS()
class THEASCENDANCE_API USpellInfoDisplay : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	void Init(const USpellDataEntryObject* spellDataEntry);

private:
	void DisplaySpellData();

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> m_SpellName = nullptr;	
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> m_SpellCooldown = nullptr;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> m_SpellDescription = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UImage> m_SpellIcon = nullptr;

	UPROPERTY()
	TSoftObjectPtr<USpellData> m_SpellData = nullptr;
};
