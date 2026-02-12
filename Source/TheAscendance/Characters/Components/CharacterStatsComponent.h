// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TheAscendance/Characters/Enums/CharacterStat.h"
#include "CharacterStatsComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnStatChanged, ECharacterStat, float, float);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class THEASCENDANCE_API UCharacterStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCharacterStatsComponent();

	void Init();

	void AddStat(ECharacterStat stat, float maxValue);
	void SetStat(ECharacterStat stat, float amount);

	void AdjustStatByValue(ECharacterStat stat, float amount);
	void AdjustStatByPercentage(ECharacterStat stat, float percentage);
	void AdjustMaxStatByValue(ECharacterStat stat, int amount);

	int GetStatAsValue(ECharacterStat stat) const;
	int GetStatAsPercentage(ECharacterStat stat) const;
	int GetStatBaseValue(ECharacterStat stat) const;
	int GetStatMaxValue(ECharacterStat stat) const;

	// Called every frame
	virtual void TickComponent(float deltaTime, ELevelTick tickType, FActorComponentTickFunction* thisTickFunction) override;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void ExecuteBindings(ECharacterStat stat);

	void LogStatWarning(ECharacterStat stat);
	void LogStatError(ECharacterStat stat);

public:
	FOnStatChanged OnStatChanged;

private:
	UPROPERTY()
	TMap<ECharacterStat, float> m_StatsBase;
	UPROPERTY()
	TMap<ECharacterStat, int> m_StatsMax;
	UPROPERTY()
	TMap<ECharacterStat, float> m_Stats;

	//---- HEALTH ----
	UPROPERTY(EditDefaultsOnly, Category = "Stats | Health", meta = (DisplayName = "Base Health"))
	float m_BaseHealth = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Stats | Health", meta = (DisplayName = "Health Regeneration Per Tick"))
	float m_HealthRegenPerTick = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Stats | Health", meta = (DisplayName = "Health Regeneration Delay"))
	float m_HealthRegenDelay = 0;
	float m_HealthRegenTimer = 0;

	//--- STAMINA ----
	UPROPERTY(EditDefaultsOnly, Category = "Stats | Stamina", meta = (DisplayName = "Base Stamina"))
	float m_BaseStamina = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Stats | Stamina", meta = (DisplayName = "Stamina Regeneration Per Tick"))
	float m_StaminaRegenPerTick = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Stats | Stamina", meta = (DisplayName = "Stamina Regeneration Delay"))
	float m_StaminaRegenDelay = 0;
	float m_StaminaRegenTimer = 0;

	//--- MANA ----
	UPROPERTY(EditDefaultsOnly, Category = "Stats | Mana", meta = (DisplayName = "Base Mana"))
	float m_BaseMana = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Stats | Mana", meta = (DisplayName = "Mana Regeneration Per Tick"))
	float m_ManaRegenPerTick = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Stats | Mana", meta = (DisplayName = "Mana Regeneration Delay"))
	float m_ManaRegenDelay = 0;
	float m_ManaRegenTimer = 0;

	//--- SPEED ---
	UPROPERTY(EditDefaultsOnly, Category = "Stats | Speed", meta = (DisplayName = "Base Walk Speed"))
	float m_BaseWalkSpeed = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Stats | Speed", meta = (DisplayName = "Sprint Speed Bonus"))
	float m_SprintSpeedBonus = 0;
	UPROPERTY(EditDefaultsOnly, Category = "Stats | Speed", meta = (DisplayName = "Crouch Speed Penalty"))
	float m_CrouchSpeedPenalty = 0;
};
