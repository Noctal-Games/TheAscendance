// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "SpellInventoryEntry.generated.h"

class UImage;
struct FSpellTableData;

UCLASS()
class THEASCENDANCE_API USpellInventoryEntry : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnListItemObjectSet(UObject* listItemObject) override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<UImage> m_SpellIcon = nullptr;
};

//void Init(const TSharedPtr<FSpellTableData> spellData);

//void SetIcon();

//TSharedPtr<FSpellTableData> m_SpellData = nullptr;
//TSoftObjectPtr<UTexture2D> m_SpellIconTexture = nullptr;