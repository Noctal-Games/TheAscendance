// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "Grimoire.generated.h"

class USpellInventoryGrid;

UCLASS()
class THEASCENDANCE_API UGrimoire : public UCommonActivatableWidget
{
	GENERATED_BODY()
	
public:
	virtual bool NativeOnHandleBackAction() override;

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget))
	TObjectPtr<USpellInventoryGrid> m_SpellInventoryGrid = nullptr;
};
