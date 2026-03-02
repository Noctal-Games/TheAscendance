// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameplayTagContainer.h"
#include "SpellLoadoutDisplay.generated.h"

class USpellLoadoutIcon;
struct FSpellTableData;

UCLASS()
class THEASCENDANCE_API USpellLoadoutDisplay : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	void Init(const TArray<FGameplayTag>& equippedSpellTags, const TArray<TSharedPtr<FSpellTableData>>& equippedSpellTableData);

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<USpellLoadoutIcon> m_SpellOneIcon = nullptr;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<USpellLoadoutIcon> m_SpellTwoIcon = nullptr;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<USpellLoadoutIcon> m_SpellThreeIcon = nullptr;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<USpellLoadoutIcon> m_SpellFourIcon = nullptr;
};
