// Fill out your copyright notice in the Description page of Project Settings.


#include "SpellLoadoutDisplay.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "Grimoire.h"
#include "TheAscendance/Spells/Structs/SpellData.h"
#include "TheAscendance/UI/Data/SpellDataEntryObject.h"
#include "SpellLoadoutIcon.h"

void USpellLoadoutDisplay::Init(const TArray<FGameplayTag>& equippedSpellTags, const TArray<TSharedPtr<FSpellTableData>>& equippedSpellTableData)
{
	TMap<FGameplayTag, TSharedPtr<FSpellTableData>> spellMap;

	for (const auto& data : equippedSpellTableData)
	{
		if (data != nullptr)
		{
			spellMap.Add(data->SpellTag, data);
		}
	}

	for (int i = 0; i < equippedSpellTags.Num(); i++)
	{
		if (i >= 4)
		{
			LOG_ERROR("More than 4 equipped spells.");
			break;
		}

		const FGameplayTag& tag = equippedSpellTags[i];

		if (tag.IsValid() == false || tag == FGameplayTag::EmptyTag)
		{
			switch (i)
			{
			case 0:
			{
				m_SpellOneIcon->Init(nullptr);
				break;
			}
			case 1:
			{
				m_SpellTwoIcon->Init(nullptr);
				break;
			}
			case 2:
			{
				m_SpellThreeIcon->Init(nullptr);
				break;
			}
			case 3:
			{
				m_SpellFourIcon->Init(nullptr);
				break;
			}
			default:
			{
				break;
			}
			}

			continue;
		}

		if (auto* entry = spellMap.Find(tag))
		{
			const auto& spell = *entry;

			if(spell == nullptr)
			{
				LOG_ERROR("[SPELL LOADOUT DISPLAY] Invalid SpellTableData for tag: %s", *tag.ToString());
				continue;
			}

			USpellDataEntryObject* obj = NewObject<USpellDataEntryObject>(this);

			obj->Data = *spell;
			obj->Icon = spell->SpellIcon.LoadSynchronous();

			switch (i)
			{
				case 0:
				{
					m_SpellOneIcon->Init(obj);
					break;
				}
				case 1:
				{
					m_SpellTwoIcon->Init(obj);
					break;
				}
				case 2:
				{
					m_SpellThreeIcon->Init(obj);
					break;
				}
				case 3:
				{
					m_SpellFourIcon->Init(obj);
					break;
				}
				default:
				{
					break;
				}
			}
		}
		else
		{
			LOG_ERROR("[SPELL LOADOUT DISPLAY] No data for tag: %s", *tag.ToString());
		}
	}
}

//for(const auto& spellData : equippedSpellTableData)
//{
//	if (spellData == nullptr)
//	{
//		LOG_ERROR("[SPELL LOADOUT DISPLAY] Invalid SpellTableData");
//		continue;
//	}

//	const int index = equippedSpellTags.IndexOfByKey(spellData->SpellTag);

//	if (index == INDEX_NONE)
//	{
//		LOG_ERROR("[SPELL LOADOUT DISPLAY] Equipped spell mismatc. EquippedSpellTags doesn't contain the tag of an EquippedSpellTableData");
//		continue;
//	}

//	USpellDataEntryObject* obj = NewObject<USpellDataEntryObject>(this);

//	obj->Data = *spellData;
//	obj->Icon = spellData->SpellIcon.LoadSynchronous();

//	switch (index)
//	{
//		case 0:
//		{
//			m_SpellOne->Init(obj);
//			break;
//		}
//		case 1:
//		{
//			m_SpellTwo->Init(obj);
//			break;
//		}
//		case 2:
//		{
//			m_SpellThree->Init(obj);
//			break;
//		}
//		case 3:
//		{
//			m_SpellFour->Init(obj);
//			break;
//		}
//		default:
//		{
//			LOG_ERROR("[SPELL LOADOUT DISPLAY] More than 4 equipped spells. Only the first 4 will be displayed.");
//			break;
//		}
//	}
//}