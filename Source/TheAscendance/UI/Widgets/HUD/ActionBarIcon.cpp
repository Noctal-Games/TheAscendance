// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionBarIcon.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/StreamableFunctionLibrary.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Items/Structs/ItemData.h"
#include "TheAscendance/Abilities/Spells/Structs/SpellData.h"

#include "Components/Image.h"

void UActionBarIcon::LoadAbilityIcon(const TSoftObjectPtr<UTexture2D>& icon)
{
	m_Texture = icon;

	if (m_Texture.IsNull() == false)
	{
		UStreamableFunctionLibrary::RequestAsyncLoad(m_Texture.ToSoftObjectPath(), [this]() { SetIcon(); });
		return;
	}

	LOG_ERROR("[ACTION BAR ICON] Failed to load icon - Texture was invalid. Using default");
	ClearAbilityIcon();
}

void UActionBarIcon::ClearAbilityIcon()
{
	m_ActionIconImage->SetBrushFromTexture(EmptyActionIcon);
}

void UActionBarIcon::NativeConstruct()
{
	Super::NativeConstruct();

	ClearAbilityIcon();
}

void UActionBarIcon::NativeDestruct()
{
	Super::NativeDestruct();
}

void UActionBarIcon::SetIcon()
{
	if (m_Texture.Get() != nullptr)
	{
		m_ActionIconImage->SetBrushFromTexture(m_Texture.Get());
	}
	else
	{
		LOG_ERROR("[ACTION BAR ICON] Failed to set icon - Texture was invalid. Using default");
		ClearAbilityIcon();
	}
}
