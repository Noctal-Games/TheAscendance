// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpellLoadoutIcon.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "SpellInventoryEntry.generated.h"

class USpellDataEntryObject;
class UGrimoire;
class UImage;
struct FSpellTableData;

UCLASS()
class THEASCENDANCE_API USpellInventoryEntry : public USpellLoadoutIcon, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnListItemObjectSet(UObject* listItemObject) override;
};

//void Init(const TSharedPtr<FSpellTableData> spellData);

//void SetIcon();

//TSharedPtr<FSpellTableData> m_SpellData = nullptr;
//TSoftObjectPtr<UTexture2D> m_SpellIconTexture = nullptr;