// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"


MainMenu::MainMenu()
	: MainMenuWidgetClass(nullptr)
	, MainMenuWidgetInstance(nullptr)
	, WorldContext(nullptr)
{
}

MainMenu::~MainMenu()
{
}

void MainMenu::Initialize(UWorld* World, TSubclassOf<UUserWidget> InWidgetClass)
{
	if (!World)
	{
		return;
	}

	if (!InWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("MainMenu: No se especificó una clase de widget válida"));
		return;
	}

	WorldContext = World;
	MainMenuWidgetClass = InWidgetClass;

	MainMenuWidgetInstance = CreateWidget<UUserWidget>(World, MainMenuWidgetClass);

}

void MainMenu::Show()
{
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->AddToViewport();

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContext, 0);
		if (PlayerController)
		{
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(MainMenuWidgetInstance->TakeWidget());
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = true;
		}
	}
}

void MainMenu::Hide()
{
	if (MainMenuWidgetInstance)
	{
		MainMenuWidgetInstance->RemoveFromParent();

		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContext, 0);
		if (PlayerController)
		{
			FInputModeGameOnly InputMode;
			PlayerController->SetInputMode(InputMode);
			PlayerController->bShowMouseCursor = false;
		}
	}
}
