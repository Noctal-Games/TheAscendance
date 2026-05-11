// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "GameplayTagContainer.h"
#include "InstancedItem.generated.h"

class UItemData;

UCLASS()
class THEASCENDANCE_API AInstancedItem : public AItem
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Test();
private:
	UPROPERTY(EditInstanceOnly, meta = (DisplayName = "Instance Tag"))
	FGameplayTag m_InstanceTag;

	UPROPERTY()
	TSoftObjectPtr<UItemData> m_ItemDataAsset = nullptr;
};
