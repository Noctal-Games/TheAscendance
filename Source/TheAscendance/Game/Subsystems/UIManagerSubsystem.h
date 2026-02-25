// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIManagerSubsystem.generated.h"

UENUM()
enum class EWidgets : uint8
{
	PLAYER_HUD,
	GRIMOIRE
};

class UPlayerHUD;
class UGrimoire;

UCLASS(Blueprintable)
class THEASCENDANCE_API UUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

	virtual bool ShouldCreateSubsystem(UObject* outer) const override;

protected:
	friend class UCustomLocalPlayerSubsystem;
	UPlayerHUD* CreatePlayerHUD();
	UGrimoire* CreateGrimoire();

private:
	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Widget Defaults"))
	TMap<EWidgets, TSubclassOf<UUserWidget>> m_WidgetDefaults;
};
