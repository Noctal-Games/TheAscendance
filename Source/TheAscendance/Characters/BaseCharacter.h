// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "Interfaces/Susceptible.h"
#include "Enums/CharacterStat.h"
#include "TheAscendance/Items/Enums/WeaponType.h"
#include "TheAscendance/Spells/Interfaces/SpellCaster.h"
#include "Enums/EquippablePart.h"
#include "BaseCharacter.generated.h"

class UCharacterStatsComponent;
class UEffectHandlerComponent;
class AHeldItem;
class UCharacterTrajectoryComponent;
class ULoadoutComponent;
class USpellCasterComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCharacterDeath, ABaseCharacter*, character);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTakeDamage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHit);

DECLARE_DELEGATE_RetVal(const FVector, FGetCastStart);
DECLARE_DELEGATE_RetVal(const FVector, FGetCastForward);

UCLASS()
class THEASCENDANCE_API ABaseCharacter : public ACharacter, public ISusceptible, public ISpellCaster, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	virtual void Heal(int amount) override;
	virtual void Damage(int amount, bool triggerOnHit) override;
	virtual void ReduceStamina(int amount) override;
	virtual int GetStat(ECharacterStat stat) override;
	virtual bool IsDead() override;
	virtual void AddEffect(UCoreEffect* effect) override;
	virtual void AdjustStat(ECharacterStat stat, int amount) override;
	virtual void AdjustMaxStat(ECharacterStat stat, int amount) override;
	virtual AActor* GetSusceptibleActor() override;

	virtual void AddImmunity(const FGameplayTag& immunity) override;
	virtual void AddResistance(const FGameplayTag& resistance) override;
	virtual bool HasImmunity(const FGameplayTag& immunity) const override;
	virtual bool HasResistance(const FGameplayTag& resistance) const override;

	bool MainHandPrimaryAttack();
	bool MainHandSecondaryAttack();
	bool OffHandPrimaryAttack();
	bool OffHandSecondaryAttack();

	UFUNCTION(BlueprintCallable)
	bool IsAttacking()
	{
		return m_IsMainHandAttacking == true || m_IsOffHandAttacking == true;
	}

	UFUNCTION(BlueprintCallable)
	bool IsMainHandAttacking() { return m_IsMainHandAttacking; };
	UFUNCTION(BlueprintCallable)
	bool IsOffHandAttacking() { return m_IsOffHandAttacking; };

	UFUNCTION(BlueprintCallable)
	bool IsMainHandPrimaryAttacking();
	UFUNCTION(BlueprintCallable)
	bool IsOffHandPrimaryAttacking();

	UFUNCTION(BlueprintCallable)
	EWeaponType MainHandWeaponType();
	UFUNCTION(BlueprintCallable)
	EWeaponType OffHandWeaponType();

	UFUNCTION(BlueprintCallable)
	void EndMainHandAttack();
	UFUNCTION(BlueprintCallable)
	void EndOffHandAttack();

	virtual AActor* GetActor() override;
	const virtual FVector GetSpellOwnerLocation() override;
	const virtual FVector GetSpellOwnerForward() override;
	const virtual FVector GetCastStartLocation() override;
	const virtual FVector GetCastStartForward() override;

	virtual void GetOwnedGameplayTags(FGameplayTagContainer& tagContainer) const override;
	virtual bool HasMatchingGameplayTag(FGameplayTag tagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& tagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& tagContainer) const override;

	void CastSpell(int slot);

	UFUNCTION(BlueprintPure)
	virtual bool IsSprinting();

	virtual void SetDestination(const FVector& destination);
	void TurnTowards(const FRotator& targetRotation);

	FVector GetSocketLocation(FName socketName);

	float PlayAnimationMontage(UAnimMontage* montageToPlay, float playRate = 1.0f, FName startSection = NAME_None);


	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	friend class ULoadoutComponent;
	friend class UPlayerHUD;

	bool EquipItem(EEquippablePart part, const FGameplayTag& itemTag);
	void UnEquipItem(EEquippablePart part);

	UCharacterStatsComponent* GetCharacterStatsComponent();
	ULoadoutComponent* GetLoadoutComponent() const;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void UpdateTurnTowards();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayTags")
	FGameplayTagContainer OwnedTags;

	FGetCastStart GetCastStartFunc;
	FGetCastForward GetCastForwardFunc;

	UPROPERTY(BlueprintAssignable)
	FOnCharacterDeath OnDeath;
	UPROPERTY(BlueprintAssignable)
	FOnTakeDamage OnDamageTaken;
	UPROPERTY(BlueprintAssignable)
	FOnHit OnHit;

protected:
	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Character Stats Component"))
	TObjectPtr<UCharacterStatsComponent> m_CharacterStatsComponent = nullptr;
	UPROPERTY(meta = (DisplayName = "Effect Handler Component"))
	TObjectPtr<UEffectHandlerComponent> m_EffectHandlerComponent = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Character Trajectory Component"))
	TObjectPtr<UCharacterTrajectoryComponent> m_CharacterTrajectoryComponent = nullptr;
	UPROPERTY()
	TObjectPtr<ULoadoutComponent> m_LoadoutComponent = nullptr;
	UPROPERTY()
	TObjectPtr<USpellCasterComponent> m_SpellCasterComponent = nullptr;

	UPROPERTY()
	TObjectPtr<AHeldItem> m_MainHandItem = nullptr;
	UPROPERTY()
	TObjectPtr<AHeldItem> m_OffHandItem = nullptr;

	UPROPERTY()
	FGameplayTagContainer m_EffectImmunities;
	UPROPERTY()
	FGameplayTagContainer m_EffectResistances;

	bool m_TestEquipToggle = false;
	bool m_AnimTest = false;

	bool m_IsSprinting = false;
	bool m_IsCrouching = false;
	bool m_IsJumping = false;

private:
	bool m_IsMainHandAttacking = false;
	bool m_IsOffHandAttacking = false;
	float m_AttackTimer = 0.0f;

	FTimerHandle m_TurnTimerHandle;
	FRotator m_TurnTargetRotation = FRotator::ZeroRotator;
	float m_TurnInterpSpeed = 5.0f;
	bool m_IsTurning = false;

	FDelegateHandle m_OnSpellsUpdatedHandle;
};
