// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CustomPlayerController.generated.h"

UCLASS()
class THEASCENDANCE_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void DisplayHUD();
	void UpdateCamera(const FVector& value);

protected:
	virtual void OnPossess(APawn* pawn) override;
	virtual void OnUnPossess() override;

public:
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (ClampMin = "0.01", ClampMax = "1.0"))
	float HorizontalSensitivity;
	UPROPERTY(EditAnywhere, Category = "Settings", meta = (ClampMin = "0.01", ClampMax = "1.0"))
	float VerticalSensitivity;
};
