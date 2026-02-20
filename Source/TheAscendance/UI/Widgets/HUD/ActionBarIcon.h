// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "ActionBarIcon.generated.h"

class UImage;
class UBorder;

UCLASS()
class THEASCENDANCE_API UActionBarIcon : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:

protected:
	friend class UActionBar;

	void SetSpellTag(const FGameplayTag& spellTag);
	void SetEquipmentTag(const FGameplayTag& equipmentTag);

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
private:
	void SetIcon();
	void UpdateIcon();

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> EmptyActionIcon = nullptr;

private:
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget, DisplayName = "Action Icon"))
	TObjectPtr<UImage> m_ActionIconImage = nullptr;
	UPROPERTY(EditDefaultsOnly, meta = (BindWidget, DisplayName = "Action Icon Border"))
	TObjectPtr<UBorder> m_ActionIconBorder = nullptr;

	UPROPERTY()
	TSoftObjectPtr<UTexture2D> m_Texture = nullptr;

	FGameplayTag m_SpellTag;
	FGameplayTag m_EquipmentTag;
};
