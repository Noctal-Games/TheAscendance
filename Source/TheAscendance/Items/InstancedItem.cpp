// Fill out your copyright notice in the Description page of Project Settings.


#include "InstancedItem.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Game/Subsystems/ItemRegistrySubsystem.h"
#include "TheAscendance/Items/Structs/ItemData.h"

// Called when the game starts or when spawned
void AInstancedItem::BeginPlay()
{
	Super::BeginPlay();
	
	if (m_InstanceTag.IsValid() == false)
	{
		LOG_ERROR("[INSTANCED ITEM] Tried to create InstancedItem with invalid ItemTag");
		return;
	}

	if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
	{
		//Init(gameMode->GetItemData(m_InstanceTag));
		
		if (UItemRegistrySubsystem* registry = gameMode->GetGameInstance()->GetSubsystem<UItemRegistrySubsystem>())
		{
			if (UItemDataAsset* itemDataAsset = registry->LoadItemData(m_InstanceTag))
			{
				LOG_WARNING("[INSTANCED ITEM] Successfully loaded ItemData for InstancedItem with tag: %s", *m_InstanceTag.ToString());
			}
			else
			{
				LOG_ERROR("[INSTANCED ITEM] Failed to load ItemData for InstancedItem with tag: %s", *m_InstanceTag.ToString());
			}
		}
	}
	else
	{
		LOG_ERROR("[INSTANCED ITEM] Tried to create InstancedItem in world without PlayableGameMode");
	}
}


