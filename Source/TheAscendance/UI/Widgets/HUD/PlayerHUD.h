// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "TheAscendance/Actors/Interaction/Interfaces/Interactable.h"
#include "PlayerHUD.generated.h"

class APlayerCharacter;
class UStatBoundProgressBar;
class UActionBar;
class UWidgetSwitcher;
class UImage;

UCLASS()
class THEASCENDANCE_API UPlayerHUD : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(APlayerCharacter* ownerCharacter);
	
	UFUNCTION(BlueprintCallable)
	void BindStatBar(UStatBoundProgressBar* statBar);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

protected:
	void UpdateCrosshair(EInteractType targetType);

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget, DisplayName = "Health Bar"))
	TObjectPtr<UStatBoundProgressBar> m_HealthBar = nullptr;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget, DisplayName = "Stamina Bar"))
	TObjectPtr<UStatBoundProgressBar> m_StaminaBar = nullptr;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget, DisplayName = "Mana Bar"))
	TObjectPtr<UStatBoundProgressBar> m_ManaBar = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (BindWidget, DisplayName = "Action Bar"))
	TObjectPtr<UActionBar> m_ActionBar = nullptr;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> m_Crosshair = nullptr;

	TWeakObjectPtr<APlayerCharacter> m_OwnerCharacter = nullptr;
};
