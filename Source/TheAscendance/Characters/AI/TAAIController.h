// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TAAIController.generated.h"

/**
 * 
 */
UCLASS()
class THEASCENDANCE_API ATAAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetDestination(const FVector& destination);
	UFUNCTION(BlueprintCallable)
	void ClearDestination();

	UFUNCTION(BlueprintCallable)
	const FVector GetCurrentDestination() const;

	//Pauses movement without clearing the current path
	UFUNCTION(BlueprintCallable)
	void SetPauseMovement(bool val);

	UFUNCTION(BlueprintCallable)
	bool HasPath() const;


private:
	FVector m_CurrentDestination;
};
