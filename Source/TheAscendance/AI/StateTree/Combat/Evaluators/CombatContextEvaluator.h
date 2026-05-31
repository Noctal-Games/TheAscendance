// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeEvaluatorBase.h"
#include "TheAscendance/AI/Combat/Structs/CombatContext.h"
#include "CombatContextEvaluator.generated.h"

USTRUCT(meta = (DisplayName = "Combat Context"))
struct FCombatContextEvaluator : public FStateTreeEvaluatorCommonBase
{
	GENERATED_BODY()
	
	using FInstanceDataType = FCombatContext;

    virtual const UStruct* GetInstanceDataType() const override
    {
        return FCombatContext::StaticStruct();
    }

    virtual void TreeStart(FStateTreeExecutionContext& context) const override;
    virtual void Tick(FStateTreeExecutionContext& context, const float deltaTime) const override;
};
