// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEffect.h"
#include "CoreEffect.generated.h"

class ISusceptible;

UCLASS()
class THEASCENDANCE_API UCoreEffect : public UBaseEffect
{
	GENERATED_BODY()
	
public:
	virtual void StartEffect(ISusceptible* target) override;
	virtual void EndEffect() override;
};
