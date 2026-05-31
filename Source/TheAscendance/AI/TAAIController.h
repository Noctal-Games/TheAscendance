// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TAAIController.generated.h"

class UCustomStateTreeComponent;
class UStateTree;

UCLASS()
class THEASCENDANCE_API ATAAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	ATAAIController();

	UFUNCTION(BlueprintCallable)
	void SetDestination(const FVector& destination, float acceptanceRadius = 15.0f);
	UFUNCTION(BlueprintCallable)
	void ClearDestination();

	UFUNCTION(BlueprintCallable)
	const FVector GetCurrentDestination() const;

	//Pauses movement without clearing the current path
	UFUNCTION(BlueprintCallable)
	void SetPauseMovement(bool val);

	UFUNCTION(BlueprintCallable)
	bool HasPath() const;

	void TestStartStateTree(UStateTree* stateTree);

private:
	UPROPERTY()
	TObjectPtr<UCustomStateTreeComponent> m_StateTreeComponent = nullptr;

	FVector m_CurrentDestination;
};
