// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellLoadoutIcon.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/Subsystems/UIManagerSubsystem.h"
#include "TheAscendance/UI/Data/SpellDataEntryObject.h"
#include "Grimoire.h"

#include "Components/Image.h"

void USpellLoadoutIcon::Init(USpellDataEntryObject* spellDataEntry)
{
	m_SpellDataEntry = spellDataEntry;

	if (m_SpellDataEntry == nullptr)
	{
		LOG_ERROR("[SPELL LOADOUT ICON] Tried to Init, but the SpellDataEntryObject was invalid");
		return;
	}

	if (m_SpellDataEntry->Icon != nullptr)
	{
		m_SpellIcon->SetBrushFromTexture(m_SpellDataEntry->Icon);
	}
	else
	{
		LOG_ERROR("[SPELL LOADOUT ICON] Spell Icon was invalid");
	}
}

FReply USpellLoadoutIcon::NativeOnFocusReceived(const FGeometry& geometryEvent, const FFocusEvent& focusEvent)
{
	FReply reply = Super::NativeOnFocusReceived(geometryEvent, focusEvent);

	UpdateGrimoireSpellInfo();

	return reply;
}

void USpellLoadoutIcon::NativeOnMouseEnter(const FGeometry& geometryEvent, const FPointerEvent& pointerEvent)
{
	SetFocus();
}

FReply USpellLoadoutIcon::NativeOnMouseButtonDown(const FGeometry& geometryEvent, const FPointerEvent& pointerEvent)
{
	FReply reply = Super::NativeOnMouseButtonDown(geometryEvent, pointerEvent);

	return reply;
}

void USpellLoadoutIcon::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (UUIManagerSubsystem* uiManager = UCoreFunctionLibrary::GetUIManagerSubsystem())
	{
		m_Grimoire = uiManager->GetGrimoire();
	}
}

void USpellLoadoutIcon::UpdateGrimoireSpellInfo()
{
	LOG_ONSCREEN(-1, 5.0f, FColor::Yellow, "%s", m_SpellDataEntry ? *m_SpellDataEntry->Data.SpellName.ToString() : *FString("EMPTY"));

	if(m_Grimoire.IsValid())
	{
		m_Grimoire->UpdateDisplayedSpellInfo(m_SpellDataEntry);
	}
}
