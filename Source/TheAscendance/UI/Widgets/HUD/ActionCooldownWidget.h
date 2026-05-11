// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionCooldownWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class THEASCENDANCE_API UActionCooldownWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void StartCooldown(float currentTime, float time);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void NativeOnInitialized() override;
private:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UProgressBar> m_CooldownBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	TObjectPtr<UTextBlock> m_CooldownText;

	float m_Time = 0.0f;
	float m_Timer = 0.0f;
};
