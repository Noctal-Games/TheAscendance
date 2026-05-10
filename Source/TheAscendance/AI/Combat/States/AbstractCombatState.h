// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AbstractCombatState.generated.h"

class UCombatAIComponent;

UCLASS()
class THEASCENDANCE_API UAbstractCombatState : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init(UCombatAIComponent* ownerComponent);

	virtual void StartState() {};
	virtual void EndState() {};
	virtual void Update(float deltaTime) {};

	virtual const FString GetStateToString() const;

protected:
	UPROPERTY()
	TWeakObjectPtr<UCombatAIComponent> m_OwnerComponent = nullptr;
};
