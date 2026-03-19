// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "CustomLocalPlayerSubsystem.generated.h"

class APlayerCharacter;
class UCommonUserWidget;
class UEnhancedInputComponent;
class UInputMappingContext;
class UInputAction;
class ACustomPlayerController;
class UPlayerHUD;
class UGrimoire;

UCLASS(Blueprintable)
class THEASCENDANCE_API UCustomLocalPlayerSubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()
	
public:
	void ToggleHUDVisibility(bool isVisible);
	void SetPlayer(APlayerCharacter* player);
	void SetController(ACustomPlayerController* controller);

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
protected:
	friend class ACustomPlayerController;
	void CreateHUD();
	void SetupInput(UEnhancedInputComponent* enhancedInputComponent);

private:
	void DestroyHUD();

	//---- INPUT HANDLERS ----
	void HandleLook(const struct FInputActionValue& value);
	void HandleMove(const struct FInputActionValue& value);
	void HandleJump();
	void HandleStartSprint();
	void HandleEndSprint();
	void HandleStartCrouch();
	void HandleEndCrouch();

	void HandleMainHandPrimaryAttack();
	void HandleMainHandSecondaryAttack();

	void HandleOffhandPrimaryAttack();
	void HandleOffhandSecondaryAttack();

	void HandleInteract();

	void HandleToggleGrimoire();

	void HandleTestFunction1();
	void HandleTestFunction2();
	void HandleTestFunction3();
	
	void BindActions(UEnhancedInputComponent* enhancedInputComponent);
public:
	//---- INPUTS ----
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI | Mapping Context")
	TObjectPtr<UInputMappingContext> UIInputMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Mapping Context")
	TObjectPtr<UInputMappingContext> InputMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Global | Character Movement")
	TObjectPtr<UInputAction> ActionLook = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Global | Character Movement")
	TObjectPtr<UInputAction> ActionMove = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Global | Character Movement")
	TObjectPtr<UInputAction> ActionJump = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Global | Character Movement")
	TObjectPtr<UInputAction> ActionSprint = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Global | Character Movement")
	TObjectPtr<UInputAction> ActionCrouch = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Global | Character Actions")
	TObjectPtr<UInputAction> ActionToggleInventory = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Global | Character Actions")
	TObjectPtr<UInputAction> ActionToggleQuestMenu = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Global | Character Actions")
	TObjectPtr<UInputAction> ActionMainHandPrimaryAttack = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Global | Character Actions")
	TObjectPtr<UInputAction> ActionMainHandSecondaryAttack = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Global | Character Actions")
	TObjectPtr<UInputAction> ActionOffHandPrimaryAttack = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Global | Character Actions")
	TObjectPtr<UInputAction> ActionOffHandSecondaryAttack = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Global | Character Actions")
	TObjectPtr<UInputAction> ActionTogglePauseMenu = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Global | Character Actions")
	TObjectPtr<UInputAction> ActionInteract = nullptr;

	//Test
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Global | Test Actions")
	TObjectPtr<UInputAction> ActionTestFunction1 = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Global | Test Actions")
	TObjectPtr<UInputAction> ActionTestFunction2 = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input | Global | Test Actions")
	TObjectPtr<UInputAction> ActionTestFunction3 = nullptr;

private:
	UPROPERTY()
	TObjectPtr<UPlayerHUD> m_HUDWidget = nullptr;
	UPROPERTY()
	TObjectPtr<UGrimoire> m_GrimoireWidget = nullptr;

	TWeakObjectPtr<ACustomPlayerController> m_Controller = nullptr;
	TWeakObjectPtr<APlayerCharacter> m_PlayerCharacter = nullptr;
};
