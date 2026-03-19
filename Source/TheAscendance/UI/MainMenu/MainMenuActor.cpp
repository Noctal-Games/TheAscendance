// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/MainMenu/MainMenuActor.h"
#include "MainMenu.h"

AMainMenuActor::AMainMenuActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = false;

	MainMenuInstance = nullptr;
}

void AMainMenuActor::BeginPlay()
{
	Super::BeginPlay();

	MainMenuInstance = new MainMenu();

	if (MainMenuInstance)
	{
		MainMenuInstance->Initialize(GetWorld(), WidgetClass);
		MainMenuInstance->Show();
	}
}

void AMainMenuActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainMenuActor::ShowMenu()
{
	if (MainMenuInstance)
	{
		MainMenuInstance->Show();
	}
}

void AMainMenuActor::HideMenu()
{
	if (MainMenuInstance)
	{
		MainMenuInstance->Hide();
	}
}
