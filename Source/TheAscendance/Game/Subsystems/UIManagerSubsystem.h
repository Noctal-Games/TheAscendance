// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIManagerSubsystem.generated.h"

UENUM()
enum class EWidgets : uint8
{
	PLAYER_HUD
};

class UCommonUserWidget;

UCLASS(Blueprintable)
class THEASCENDANCE_API UUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

	virtual bool ShouldCreateSubsystem(UObject* outer) const override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Widget Defaults"))
	TMap<EWidgets, TSubclassOf<UCommonUserWidget>> m_WidgetDefaults;
};
