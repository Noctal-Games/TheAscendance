// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "TheAscendance/AI/StateTree/Generic/Structs/MovementTaskInstanceData.h"
#include "StrafeTask.generated.h"

/**
 * 
 */
USTRUCT(meta = (DisplayName = "Strafe", ToolTip = "The AI strafes around the target actor."))
struct THEASCENDANCE_API FStrafeTask : public FStateTreeTaskCommonBase
{
	GENERATED_BODY()
	
	using FInstanceDataType = FStrafeMovementTaskInstanceData;

	virtual const UStruct* GetInstanceDataType() const override
	{
		return FStrafeMovementTaskInstanceData::StaticStruct();
	}

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& context, const FStateTreeTransitionResult& transition) const override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& context, const float deltaTime) const override;
};
