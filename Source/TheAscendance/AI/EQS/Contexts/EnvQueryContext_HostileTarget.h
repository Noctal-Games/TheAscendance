// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "EnvQueryContext_HostileTarget.generated.h"

/**
 * 
 */
UCLASS()
class THEASCENDANCE_API UEnvQueryContext_HostileTarget : public UEnvQueryContext
{
	GENERATED_BODY()
	
public:
	virtual void ProvideContext(FEnvQueryInstance& queryInstance, FEnvQueryContextData& contextData) const override;
};
