// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTileView.h"
#include "SpellInventoryGrid.generated.h"

class UGrimoire;
class USpellInventoryEntry;
struct FSpellTableData;

UCLASS()
class THEASCENDANCE_API USpellInventoryGrid : public UCommonTileView
{
	GENERATED_BODY()
	
public:	
	void PopulateList(const TArray<TSharedPtr<FSpellTableData>>& spellList);

private:
	void UpdateGrid();

private:
	TArray<TSharedPtr<FSpellTableData>> m_SpellList;
};

//friend class UGrimoire;

//void ClearGrid();

//UPROPERTY(EditDefaultsOnly, meta = (DisplayAfter = "Entry Icon Default"))
//TSubclassOf<USpellInventoryEntry> m_EntryDefault = nullptr;

//UPROPERTY()
//TArray<TObjectPtr<USpellInventoryEntry>> m_EntryWidgets;
