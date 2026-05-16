// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityData.h"

FAbilityInfo UAbilityData::GetAbilityInfo()
{
	FAbilityInfo info;
	info.Tag = AbilityTag;
	info.Cooldown = Cooldown;
	info.Icon = AbilityIcon;

	return info;
}
