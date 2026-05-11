// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CombatManagerSubsystem.generated.h"

class UCombatAIComponent;

UCLASS()
class THEASCENDANCE_API UCombatManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	void RegisterAIComponent(UCombatAIComponent* component);
	void UnRegisterAIComponent(UCombatAIComponent* component);

	virtual void Initialize(FSubsystemCollectionBase& collection) override;
	virtual void Deinitialize() override;

private:
	void CleanRegistry();

private:
	UPROPERTY()
	TArray<TWeakObjectPtr<UCombatAIComponent>> m_RegisteredComponents;

	FTimerHandle m_CleanUpTimer;
};
