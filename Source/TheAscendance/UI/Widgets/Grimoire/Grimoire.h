// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "Grimoire.generated.h"

class USpellInventoryGrid;
class USpellLoadoutDisplay;
class USpellInfoDisplay;
class USpellDataEntryObject;
class ULoadoutComponent;

UCLASS()
class THEASCENDANCE_API UGrimoire : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	virtual bool NativeOnHandleBackAction() override;

	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	friend class USpellLoadoutIcon;
	friend class USpellInventoryEntry;

	void UpdateDisplayedSpellInfo(const USpellDataEntryObject* spellDataEntry);

private:
	void UpdateGrimoire(const TArray<FGameplayTag>& spellTags);

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<USpellInventoryGrid> m_SpellInventoryGrid = nullptr;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<USpellLoadoutDisplay> m_SpellLoadoutDisplay = nullptr;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<USpellInfoDisplay> m_SpellInfoDisplay = nullptr;

	UPROPERTY()
	TWeakObjectPtr<ULoadoutComponent> m_PlayerLoadout = nullptr;
};
