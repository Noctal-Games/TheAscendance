// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectLoader.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/Subsystems/DataHandlerSubsystem.h"
#include "TheAscendance/Effects/EffectFactory.h"
#include "TheAscendance/Effects/Structs/EffectData.h"

void UEffectLoader::Init()
{
	m_EffectFactory = MakeUnique<EffectFactory>();

	if (UDataHandlerSubsystem* dataHandler = UCoreFunctionLibrary::GetDataHandlerSubsystem())
	{
		m_EffectTable = dataHandler->LoadData(EDataGroup::EFFECTS);

		if (m_EffectTable == nullptr)
		{
			LOG_ERROR("EffectLoader failed to load Effect DataTable");
		}
		else
		{
			LOG_INFO("EffectLoader succeeded to load Effect DataTable");
		}
	}
}

UBaseEffect* UEffectLoader::CreateEffectFromTag(const FGameplayTag& effectTag)
{
	if (m_EffectFactory == nullptr)
	{
		LOG_ERROR("EffectLoader tried to create Effect without an EffectFactory");
		return nullptr;
	}

	if (UEffectData* effectData = LoadEffectData(effectTag))
	{
		return m_EffectFactory->CreateEffect(effectData);
	}

	LOG_ERROR("EffectLoader failed to create EffectData for Effect Tag: %s", *effectTag.ToString());
	return nullptr;
}

UBaseEffect* UEffectLoader::CreateEffectFromEffectData(UEffectData* effectData)
{
	if (effectData == nullptr || m_EffectFactory == nullptr)
	{
		LOG_ERROR("EffectLoader tried to create Effect without a valid EffectData or without a EffectFactory");
		return nullptr;
	}

	return m_EffectFactory->CreateEffect(effectData);
}

UEffectData* UEffectLoader::LoadEffectData(const FGameplayTag& effectTag)
{
	if (m_EffectTable == nullptr)
	{
		LOG_ERROR("EffectLoader tried to load EffectData without a valid EffectTable");
		return nullptr;
	}

	static const FString contextString(TEXT("Effect Context String"));

	TArray<FEffectTableData*> effectStructs;
	m_EffectTable->GetAllRows(contextString, effectStructs);

	for (const auto data : effectStructs)
	{
		if (data->EffectTag != effectTag)
		{
			continue;
		}

		FSoftObjectPath path(data->EffectData.ToSoftObjectPath());
		UObject* pathObject = path.ResolveObject();

		if (pathObject == nullptr)
		{
			pathObject = path.TryLoad();
		}

		if (pathObject == nullptr)
		{
			LOG_ERROR("EffectLoader failed to load EffectData for Effect Tag: %s", *effectTag.ToString());
			return nullptr;
		}

		if (UEffectData* effectData = Cast<UEffectData>(pathObject))
		{
			return effectData;
		}
	}

	LOG_ERROR("EffectLoader failed to load EffectData for Effect Tag: %s", *effectTag.ToString());
	return nullptr;
}
