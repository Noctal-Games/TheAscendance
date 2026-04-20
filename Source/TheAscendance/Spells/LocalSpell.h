// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseSpell.h"
#include "LocalSpell.generated.h"

class ULocalSpellData;

UCLASS()
class THEASCENDANCE_API ULocalSpell : public UBaseSpell
{
	GENERATED_BODY()
	
public:
	virtual void Execute() override;

	virtual void ProcessHit(const FVector& spellHitLocation) override;

private:
	UPROPERTY()
	TWeakObjectPtr<ULocalSpellData> m_SpellData = nullptr;
};
