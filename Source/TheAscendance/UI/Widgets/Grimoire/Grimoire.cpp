// Fill out your copyright notice in the Description page of Project Settings.


#include "Grimoire.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/Subsystems/UIManagerSubsystem.h"
#include "SpellInventoryGrid.h"
#include "SpellLoadoutDisplay.h"
#include "SpellInfoDisplay.h"
#include "TheAscendance/Abilities/Spells/Structs/SpellData.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Characters/Player/PlayerCharacter.h"
#include "TheAscendance/Characters/Components/LoadoutComponent.h"
#include "SpellLoadoutIcon.h"
#include "TheAscendance/UI/Data/SpellDataEntryObject.h"

bool UGrimoire::NativeOnHandleBackAction()
{
	if (m_SelectionOne != nullptr)
	{
		m_SelectionOne = nullptr;
		m_SelectionTwo = nullptr;
	}
	else
	{
		if (m_PlayerLoadout == nullptr)
		{
			LOG_ERROR("[GRIMOIRE] Unable to find reference to Player LoadoutComponent during Construct");
		}
		else
		{
			TArray<FGameplayTag> spellTags;
			m_SpellLoadoutDisplay->GetSelectedSpellTags(spellTags);
			m_PlayerLoadout->SetSpells(spellTags);
		}

		DeactivateWidget();
	}

	return true;
}

void UGrimoire::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (UUIManagerSubsystem* uiManager = UCoreFunctionLibrary::GetUIManagerSubsystem())
	{
		uiManager->SetGrimoireRef(this);
	}
}

void UGrimoire::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerCharacter* player = UCoreFunctionLibrary::GetPlayerCharacter();

	if(player == nullptr)
	{
		LOG_ERROR("[GRIMOIRE] Unable to find reference to Player during Construct");
		return;
	}

	m_PlayerLoadout = player->GetLoadoutComponent();

	if (m_PlayerLoadout == nullptr)
	{
		LOG_ERROR("[GRIMOIRE] Unable to find reference to Player LoadoutComponent during Construct");
		return;
	}

	m_PlayerLoadout->OnSpellsUpdated.AddUObject(this, &UGrimoire::UpdateGrimoire);

	UpdateGrimoire(m_PlayerLoadout->GetSpellTags());
}

void UGrimoire::NativeDestruct()
{
	if (m_PlayerLoadout != nullptr)
	{
		m_PlayerLoadout->OnSpellsUpdated.RemoveAll(this);
	}

	Super::NativeDestruct();
}


void UGrimoire::UpdateDisplayedSpellInfo(USpellLoadoutIcon* spellIcon)
{
	m_FocussedIcon = spellIcon;

	if (m_FocussedIcon != nullptr)
	{
		m_SpellInfoDisplay->Init(m_FocussedIcon->GetSpellDataEntry());
	}
}

void UGrimoire::UpdateSelection(USpellLoadoutIcon* spellIcon)
{
	if (m_SelectionOne == nullptr)
	{
		m_SelectionOne = spellIcon;
		return;
	}

	if (m_SelectionOne == spellIcon)
	{
		return;
	}

	m_SelectionTwo = spellIcon;
	HandleSelection();
}

void UGrimoire::UpdateGrimoire(const TArray<FGameplayTag>& spellTags)
{
	if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
	{
		TArray<TSharedPtr<FSpellTableData>> spellDataEntries = gameMode->GetAllSpellTableDataEntries();

		TArray<TSharedPtr<FSpellTableData>> equippedSpellEntries;
		TArray<TSharedPtr<FSpellTableData>> unequippedSpellEntries;

		for (const TSharedPtr<FSpellTableData>& spell : spellDataEntries)
		{
			if (spellTags.Contains(spell->SpellTag))
			{
				equippedSpellEntries.Add(spell);
			}
			else
			{
				unequippedSpellEntries.Add(spell);
			}
		}

		m_SpellInventoryGrid->PopulateList(unequippedSpellEntries);
		m_SpellLoadoutDisplay->Init(m_PlayerLoadout->GetSpellTags(), equippedSpellEntries);
	}
	else
	{
		LOG_ERROR("[GRIMOIRE] Failed to get PlayableGameMode reference in NativeConstruct");
	}
}

void UGrimoire::HandleSelection()
{
	if (m_SelectionOne == nullptr || m_SelectionTwo == nullptr)
	{
		m_SelectionOne = nullptr;
		m_SelectionTwo = nullptr;

		LOG_ONSCREEN(-1, 5.0f, FColor::Yellow, "SELECTION FAILED");
		return;
	}

	USpellDataEntryObject* entry = m_SelectionOne->GetSpellDataEntry();

	m_SelectionOne->Init(m_SelectionTwo->GetSpellDataEntry());
	m_SelectionTwo->Init(entry);

	m_SelectionOne = nullptr;
	m_SelectionTwo = nullptr;

	m_SpellInventoryGrid->RequestRefresh();
	LOG_ONSCREEN(-1, 5.0f, FColor::Yellow, "SELECTION");
}
