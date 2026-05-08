// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheAscendance/Characters/BaseCharacter.h"
#include "TheAscendance/AI/Actions/Attacks/Structs/AttackData.h"
#include "BaseEnemy.generated.h"

class ATAAIController;
class UHSMAgentComponent;
class USkeletalMesh;
class UAnimInstance;
class AWaypointRoute;
class UEnemyData;
struct FEnemyAbilityData;

UCLASS()
class THEASCENDANCE_API ABaseEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy();

	void Init(const UEnemyData* data);
	void SetSkeletalMesh();

	virtual void Damage(int amount, bool triggerOnHit) override;

	virtual void SetDestination(const FVector& destination) override;
	void SetLocationToInvestigate(const FVector& location);
	bool HasPath() const;

	//Temp until better setup
	void SetWaypointRoute(AWaypointRoute* route);

	void SetFocus(AActor* target);
	void ClearFocus();

	bool IsSoundHeard(float soundWeight) const;
	bool IsInCombat() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void InitAbilityData(const FEnemyAbilityData& abilityData);

private:
	UPROPERTY()
	TSoftObjectPtr<USkeletalMesh> m_SkeletalMesh = nullptr;
	UPROPERTY()
	TSoftClassPtr<UAnimInstance> m_AnimationBP = nullptr;

	TWeakObjectPtr<ATAAIController> m_Controller = nullptr;

	UPROPERTY()
	TObjectPtr<UHSMAgentComponent> m_Agent = nullptr;

	FGameplayTag m_EnemyTag;
};
