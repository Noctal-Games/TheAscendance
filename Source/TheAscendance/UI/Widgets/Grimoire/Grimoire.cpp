// Fill out your copyright notice in the Description page of Project Settings.


#include "Grimoire.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "SpellInventoryGrid.h"
#include "TheAscendance/Spells/Structs/SpellData.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"

bool UGrimoire::NativeOnHandleBackAction()
{
	LOG_ONSCREEN(-1, 5.0f, FColor::Green, "GRIMOIRE BACK");
	DeactivateWidget();

	return true;
}

void UGrimoire::NativeConstruct()
{
	Super::NativeConstruct();

	if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
	{
		TArray<TSharedPtr<FSpellTableData>> spellDataEntries = gameMode->GetAllSpellTableDataEntries();
		TArray<TSharedPtr<FSpellTableData>> equippedSpellDataEntries;

		for (const auto spellData : spellDataEntries)
		{
			//if equipped, add to equipped array and remove from entries array
		}

		m_SpellInventoryGrid->PopulateList(spellDataEntries);
	}
	else
	{
		LOG_ERROR("[GRIMOIRE] Failed to get PlayableGameMode reference in NativeConstruct");
	}
}

void UGrimoire::NativeDestruct()
{
	Super::NativeDestruct();
}

//if(m_SpellInventoryGrid != nullptr)
//{
//	m_SpellInventoryGrid->ClearGrid();
//}
