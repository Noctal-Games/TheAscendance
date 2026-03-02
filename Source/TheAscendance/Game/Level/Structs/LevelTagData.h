// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetUserData.h"
#include "GameplayTagContainer.h"
#include "LevelTagData.generated.h"

UCLASS()
class THEASCENDANCE_API ULevelTagData : public UAssetUserData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Location.Level"))
	FGameplayTag LevelTag;
};
