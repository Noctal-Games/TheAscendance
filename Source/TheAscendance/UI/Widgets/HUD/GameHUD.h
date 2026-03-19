// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameHUD.generated.h"

class UCommonActivatableWidgetStack;
class UGrimoire;

UCLASS()
class THEASCENDANCE_API UGameHUD : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	friend class AGrimoireActor;
	void PushGrimoireWidget();

public:
	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "HUD Stack", BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> HUDStack = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Grimoire Default"))
	TSubclassOf<UGrimoire> GrimoireClassRef = nullptr;
};
