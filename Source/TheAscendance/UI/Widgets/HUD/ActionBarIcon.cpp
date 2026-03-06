// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionBarIcon.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Items/Structs/ItemData.h"
#include "TheAscendance/Spells//Structs/SpellData.h"

#include "Components/Image.h"

void UActionBarIcon::UpdateIcon()
{
	APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode();

	if(gameMode == nullptr)
	{
		LOG_ERROR("[ACTION BAR ICON] Failed to update icon - GameMode was invalid");
		return;
	}

	if(m_EquipmentTag.IsValid() && m_EquipmentTag != FGameplayTag::EmptyTag)
	{
		if (FWeaponData* weaponData = gameMode->GetWeaponData(m_EquipmentTag))
		{
			if(const FWeaponTypeData* weaponTypeData = gameMode->GetWeaponTypeData(weaponData->WeaponType))
			{
				if(weaponTypeData->CastsSpell == false)
				{
					if (FItemData* itemData = gameMode->GetItemData(m_EquipmentTag))
					{
						m_Texture = itemData->ItemIcon;

						if (m_Texture.IsNull() == false)
						{
							UCoreFunctionLibrary::RequestAsyncLoad(m_Texture.ToSoftObjectPath(), [this]() { SetIcon(); });
							return;
						}
					}
				}
			}
		}
	}

	if(m_SpellTag.IsValid() && m_SpellTag != FGameplayTag::EmptyTag)
	{
		if (const FSpellTableData* spellData = gameMode->GetSpellTableData(m_SpellTag))
		{
			m_Texture = spellData->SpellIcon;

			if (m_Texture.IsNull() == false)
			{
				UCoreFunctionLibrary::RequestAsyncLoad(m_Texture.ToSoftObjectPath(), [this]() { SetIcon(); });
				return;
			}
		}
	}

	m_ActionIconImage->SetBrushFromTexture(EmptyActionIcon);
}

void UActionBarIcon::SetSpellTag(const FGameplayTag& spellTag)
{
	if(m_SpellTag != spellTag)
	{
		m_SpellTag = spellTag;
		UpdateIcon();
	}
}

void UActionBarIcon::SetEquipmentTag(const FGameplayTag& equipmentTag)
{
	LOG_ONSCREEN(-1, 5.0f, FColor::Green, "%s", *equipmentTag.ToString());

	if(m_EquipmentTag != equipmentTag)
	{
		m_EquipmentTag = equipmentTag;
		UpdateIcon();
	}
}

void UActionBarIcon::NativeConstruct()
{
	Super::NativeConstruct();

	m_ActionIconImage->SetBrushFromTexture(EmptyActionIcon);
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
		m_ActionIconImage->SetBrushFromTexture(EmptyActionIcon);
	}
}
