// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TheAscendance/Spells/Structs/SpellData.h"
#include "SpellDataEntryObject.generated.h"

UCLASS()
class THEASCENDANCE_API USpellDataEntryObject : public UObject
{
	GENERATED_BODY()

public:
    FSpellTableData Data;

    UPROPERTY()
    TObjectPtr<UTexture2D> Icon;
};
