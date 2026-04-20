// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellInfoDisplay.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Abilities/Spells/Structs/SpellData.h"
#include "TheAscendance/UI/Data/SpellDataEntryObject.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

void USpellInfoDisplay::Init(const USpellDataEntryObject* spellDataEntry)
{
	if (spellDataEntry == nullptr)
	{
		m_SpellName->SetText(FText::GetEmpty());
		m_SpellDescription->SetText(FText::GetEmpty());
		m_SpellIcon->SetBrushFromTexture(nullptr);

		m_SpellData = nullptr;		
		return;
	}

	const FSpellTableData& data = spellDataEntry->Data;

	m_SpellName->SetText(FText::FromName(data.SpellName));
	m_SpellDescription->SetText(FText::FromString(data.SpellDescription));
	m_SpellIcon->SetBrushFromTexture(spellDataEntry->Icon);

	m_SpellData = data.SpellData;

	if (m_SpellData.IsNull() == false)
	{
		UCoreFunctionLibrary::RequestAsyncLoad(m_SpellData.ToSoftObjectPath(), [this]() { DisplaySpellData(); });
	}
}

void USpellInfoDisplay::DisplaySpellData()
{
	if (m_SpellData.IsValid() == false)
	{
		LOG_ERROR("[SPELL INFO DISPLAY] Tried to DisplaySpellData but SpellData was invalid");
		m_SpellCooldown->SetText(FText::AsNumber(-1));
		return;
	}

	FString cooldownString;
	cooldownString.Append("Cooldown: ");
	cooldownString.Append(FString::SanitizeFloat(m_SpellData->SpellCooldown));
	cooldownString.Append("s");

	m_SpellCooldown->SetText(FText::FromString(cooldownString));
}
