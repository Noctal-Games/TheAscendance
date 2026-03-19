// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class UUserWidget;
class UWorld;

class MainMenu
{
public:
	MainMenu();
	~MainMenu();

	// Inicializa el widget con la clase del blueprint
	void Initialize(UWorld* World, TSubclassOf<UUserWidget> InWidgetClass);

	// Muestra el widget en pantalla
	void Show();

	// Oculta el widget de pantalla
	void Hide();

private:
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UUserWidget* MainMenuWidgetInstance;
	
	// Referencia al mundo
	UWorld* WorldContext;
};
