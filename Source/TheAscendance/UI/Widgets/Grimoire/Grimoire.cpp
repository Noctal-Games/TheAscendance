// Fill out your copyright notice in the Description page of Project Settings.


#include "Grimoire.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/Subsystems/UIManagerSubsystem.h"
#include "SpellInventoryGrid.h"
#include "SpellLoadoutDisplay.h"
#include "SpellInfoDisplay.h"
#include "TheAscendance/Spells/Structs/SpellData.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Characters/Player/PlayerCharacter.h"
#include "TheAscendance/Characters/Components/LoadoutComponent.h"

bool UGrimoire::NativeOnHandleBackAction()
{
	DeactivateWidget();
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

	if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
	{
		TArray<TSharedPtr<FSpellTableData>> spellDataEntries = gameMode->GetAllSpellTableDataEntries();

		TArray<TSharedPtr<FSpellTableData>> equippedSpellEntries;
		TArray<TSharedPtr<FSpellTableData>> unequippedSpellEntries;

		TSet<FGameplayTag> equippedTags(m_PlayerLoadout->GetSpellTags());

		for (const TSharedPtr<FSpellTableData>& spell : spellDataEntries)
		{
			if (equippedTags.Contains(spell->SpellTag))
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


void UGrimoire::UpdateDisplayedSpellInfo(const USpellDataEntryObject* spellDataEntry)
{
	m_SpellInfoDisplay->Init(spellDataEntry);
}
