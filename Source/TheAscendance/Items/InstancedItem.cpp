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


	if (UItemRegistrySubsystem* registry = UCoreFunctionLibrary::GetItemRegistrySubsystem())
	{
		if (const TSoftObjectPtr<UItemData>* itemRef = registry->GetItemRef(m_InstanceTag))
		{
			m_ItemDataAsset = *itemRef;

			TWeakObjectPtr<AInstancedItem> weakThis(this);

			UStreamableFunctionLibrary::RequestAsyncLoad(m_ItemDataAsset.ToSoftObjectPath(), [weakThis]()
				{
					if (weakThis.IsValid())
					{
						weakThis->Test();
					}
				}
			);
		}
		else
		{
			LOG_ERROR("[INSTANCED ITEM] Failed to find ItemData asset for tag: %s", *m_InstanceTag.ToString());
			return;
		}
	}
}

void AInstancedItem::Test()
{
	if (m_ItemDataAsset.IsValid())
	{
		Init(m_ItemDataAsset.Get());
	}
}
