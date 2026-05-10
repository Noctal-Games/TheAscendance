// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionCooldownWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UActionCooldownWidget::StartCooldown(float currentTime, float time)
{
	if (time <= 0)
	{
		m_CooldownBar->SetVisibility(ESlateVisibility::Hidden);
		m_CooldownText->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	m_Time = time;
	m_Timer = currentTime;

	m_CooldownBar->SetVisibility(ESlateVisibility::HitTestInvisible);
	m_CooldownText->SetVisibility(ESlateVisibility::HitTestInvisible);
}

void UActionCooldownWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (m_Time <= 0)
	{
		return;
	}

	m_Timer -= InDeltaTime;

	m_CooldownBar->SetPercent(m_Timer / m_Time);

	FString string = "";
	string.Append(FString::Printf(TEXT("%.2f"), m_Timer));

	m_CooldownText->SetText(FText::FromString(string));

	if (m_Timer <= 0)
	{
		m_Time = 0;
		m_CooldownBar->SetVisibility(ESlateVisibility::Hidden);
		m_CooldownText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UActionCooldownWidget::NativeOnInitialized()
{
	m_Time = 0;
	m_CooldownBar->SetVisibility(ESlateVisibility::Hidden);
	m_CooldownText->SetVisibility(ESlateVisibility::Hidden);
}
