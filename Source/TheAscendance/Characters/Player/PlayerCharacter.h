// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Characters/BaseCharacter.h"
#include "TheAscendance/Actors/Interaction/Enums/InteractType.h"
#include "PlayerCharacter.generated.h"

class UPlayerMovementComponent;
class UCameraComponent;
class ACustomPlayerController;
class ISpell;
class USoundBase;
class IInteractable;

DECLARE_DELEGATE_OneParam(FOnInteractTargetChanged, EInteractType);

UCLASS()
class THEASCENDANCE_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	void SetPlayerController(ACustomPlayerController* PlayerController);
	ACustomPlayerController* GetPlayerController();

	void Interact();

	UFUNCTION(BlueprintCallable)
	bool PickupItem(const FGameplayTag& itemTag, int amount);

	void SetIsSprinting(bool val);

	void SetIsCrouching(bool val);
	bool IsCrouching();
	bool IsCrouched();

	void SetIsJumping();
	virtual bool CanJumpInternal_Implementation() const override;
	virtual void OnJumped_Implementation() override;

	float GetDefaultCapsuleHeight();

	void UpdateCrouchCamera(float deltaTime);
	virtual void OnMovementModeChanged(EMovementMode prevMovementMode, uint8 previousCustomMode) override;

	UCameraComponent* GetCamera();

	const virtual FVector GetCastStartForward() override;

	void TestFunction1();
	void TestFunction2();
	void TestFunction3();

	UFUNCTION(BlueprintCallable)
	void TestSetSpells(const TArray<FGameplayTag>& spellTags);

	virtual bool IsSprinting() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void HandleLookAtInteractions();
	void CheckForInteractTarget(bool iHit, const FHitResult& hit);

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (DisplayName = "Player Camera"))
	TObjectPtr<UCameraComponent> m_Camera = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float InteractRange = 500.0f;

protected:
	friend class UPlayerHUD;

	FOnInteractTargetChanged m_OnInteractTargetChanged;

private:
	UPROPERTY()
	TScriptInterface<IInteractable> m_InteractTarget = nullptr;

	UPROPERTY()
	TObjectPtr<UPlayerMovementComponent> m_MovementComponent = nullptr;
	UPROPERTY()
	TObjectPtr<ACustomPlayerController> m_PlayerController = nullptr;

	float m_CrouchCapsuleHeight = 0.0f;
	float m_CurrentCapsuleHeight = 0.0f;
	float m_DefaultCapsuleHeight = 0.0f;
	float m_DefaultCapsuleRadius = 0.0f;

	//Test
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> m_TestSound = nullptr;
};
