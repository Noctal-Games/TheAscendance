// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameHUD.generated.h"

class UCommonActivatableWidgetStack;

UCLASS()
class THEASCENDANCE_API UGameHUD : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "HUD Stack", BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> HUDStack = nullptr;
};
