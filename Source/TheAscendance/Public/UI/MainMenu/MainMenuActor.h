// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MainMenuActor.generated.h"

class MainMenu;
class UUserWidget;

UCLASS()
class THEASCENDANCE_API AMainMenuActor : public AActor
{
	GENERATED_BODY()
	
public:
	AMainMenuActor();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

protected:
	MainMenu* MainMenuInstance;

	UPROPERTY(EditAnywhere, Category = "MainMenu")
	TSubclassOf<UUserWidget> WidgetClass;

public:
	UFUNCTION(BlueprintCallable, Category = "MainMenu")
	void ShowMenu();

	UFUNCTION(BlueprintCallable, Category = "MainMenu")
	void HideMenu();
};
