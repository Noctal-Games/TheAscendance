// Fill out your copyright notice in the Description page of Project Settings.


#include "InstancedItem.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"

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
		Init(gameMode->GetItemData(m_InstanceTag));
	}
	else
	{
		LOG_ERROR("[INSTANCED ITEM] Tried to create InstancedItem in world without PlayableGameMode");
	}
}


