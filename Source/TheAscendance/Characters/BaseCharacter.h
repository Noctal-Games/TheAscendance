// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Susceptible.h"
#include "Enums/CharacterStat.h"
#include "TheAscendance/Items/Enums/WeaponType.h"
#include "TheAscendance/Spells/Interfaces/SpellCaster.h"
#include "BaseCharacter.generated.h"

class UCharacterStatsComponent;
class AHeldItem;

UCLASS()
class THEASCENDANCE_API ABaseCharacter : public ACharacter, public ISusceptible, public ISpellCaster
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	virtual void Heal(int amount) override;
	virtual void Damage(int amount) override;
	virtual void ReduceStamina(int amount) override;
	virtual int GetStat(ECharacterStat stat) override;
	virtual bool IsDead() override;

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

	virtual AActor* GetSpellOwner() override;
	const virtual FVector GetSpellOwnerLocation() override;
	const virtual FVector GetSpellOwnerForward() override;
	const virtual FVector GetCastStartLocation() override;
	const virtual FVector GetCastStartForward() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Character Stats Component"))
	TObjectPtr<UCharacterStatsComponent> m_CharacterStatsComponent = nullptr;

protected:
	UPROPERTY()
	TObjectPtr<AHeldItem> m_MainHandItem = nullptr;
	UPROPERTY()
	TObjectPtr<AHeldItem> m_OffHandItem = nullptr;

	bool m_TestEquipToggle = false;
	bool m_AnimTest = false;

private:
	bool m_IsMainHandAttacking = false;
	bool m_IsOffHandAttacking = false;
	float m_AttackTimer = 0.0f;
};
