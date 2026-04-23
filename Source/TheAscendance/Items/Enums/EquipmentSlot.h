// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//Add body parts as gear is added
UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	ONE_HAND UMETA(DisplayName = "One Handed"),
	TWO_HAND UMETA(DisplayName = "Two Handed")
};
