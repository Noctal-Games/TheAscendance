// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UBaseEffect;
class UEffectData;

class THEASCENDANCE_API EffectFactory
{
public:
	EffectFactory() = default;
	~EffectFactory() = default;

	UBaseEffect* CreateEffect(UEffectData* effectData);
};
