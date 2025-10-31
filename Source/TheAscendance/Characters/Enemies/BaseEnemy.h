// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Characters/BaseCharacter.h"
#include "BaseEnemy.generated.h"

class ATAAIController;
class UHSMAgentComponent;
class USkeletalMesh;
class UAnimBlueprintGeneratedClass;
struct FEnemyTableData;

UCLASS()
class THEASCENDANCE_API ABaseEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy();

	void Init(FEnemyTableData* data);
	void SetSkeletalMesh();

	void SetDestination(const FVector& destination);
	bool HasPath() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TSoftObjectPtr<USkeletalMesh> m_SkeletalMesh;
	UPROPERTY()
	TSoftClassPtr<UAnimBlueprintGeneratedClass> m_AnimationBP;

	TWeakObjectPtr<ATAAIController> m_Controller = nullptr;

	UPROPERTY()
	TObjectPtr<UHSMAgentComponent> m_Agent = nullptr;
};
