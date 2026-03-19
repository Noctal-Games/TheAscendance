// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UIManagerSubsystem.generated.h"

class UGameHUD;
class UCommonActivatableWidget;
class ACustomPlayerController;
class UGrimoire;

UCLASS(Blueprintable)
class THEASCENDANCE_API UUIManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

	virtual bool ShouldCreateSubsystem(UObject* outer) const override;

	UGameHUD* GetGameHUD();

protected:
	friend class USpellLoadoutIcon;
	friend class UGrimoire;

	UGrimoire* GetGrimoireRef();
	void SetGrimoireRef(UGrimoire* grimoire);
private:
	friend class UCustomLocalPlayerSubsystem;
	friend class UGameHUD;

	void CreateGameHUD(ACustomPlayerController* controller);

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameHUD> GameHUDDefault = nullptr;

private:
	UPROPERTY()
	TObjectPtr<UGameHUD> m_GameHUD = nullptr;

	UPROPERTY()
	TWeakObjectPtr<UGrimoire> m_Grimoire = nullptr;
};
