// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellLoadoutIcon.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/Subsystems/UIManagerSubsystem.h"
#include "TheAscendance/UI/Data/SpellDataEntryObject.h"
#include "Grimoire.h"

#include "Components/Image.h"
#include "Components/Button.h"

void USpellLoadoutIcon::Init(USpellDataEntryObject* spellDataEntry)
{
	m_SpellDataEntry = spellDataEntry;

	if (m_SpellDataEntry == nullptr)
	{
		m_SpellIcon->SetBrushFromTexture(nullptr);
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
	Super::NativeOnMouseEnter(geometryEvent, pointerEvent);

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
		m_Grimoire = uiManager->GetGrimoireRef();
	}
}

void USpellLoadoutIcon::NativeConstruct()
{
	Super::NativeConstruct();

	m_Button->OnHovered.AddDynamic(this, &USpellLoadoutIcon::UpdateGrimoireSpellInfo);
	m_Button->OnClicked.AddDynamic(this, &USpellLoadoutIcon::HandleGrimoireSpellSelection);
}

void USpellLoadoutIcon::NativeDestruct()
{
	m_Button->OnHovered.RemoveAll(this);
	m_Button->OnClicked.RemoveAll(this);

	Super::NativeDestruct();
}

USpellDataEntryObject* USpellLoadoutIcon::GetSpellDataEntry()
{
	return m_SpellDataEntry;
}

void USpellLoadoutIcon::UpdateGrimoireSpellInfo()
{
	if (m_SpellDataEntry == nullptr)
	{
		return;
	}

	if(m_Grimoire.IsValid())
	{
		m_Grimoire->UpdateDisplayedSpellInfo(this);
	}
}

void USpellLoadoutIcon::HandleGrimoireSpellSelection()
{
	if (m_Grimoire.IsValid())
	{
		m_Grimoire->UpdateSelection(this);
	}
}
