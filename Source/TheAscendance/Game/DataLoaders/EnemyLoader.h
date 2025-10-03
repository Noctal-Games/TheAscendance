// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnemyLoader.generated.h"

class ABaseEnemy;

UCLASS()
class THEASCENDANCE_API UEnemyLoader : public UObject
{
	GENERATED_BODY()
	
public:
	void Init();
	void SetEnemyDefault(const TSubclassOf<ABaseEnemy>& enemyDefault);

	ABaseEnemy* CreateEnemyFromID(int enemyID);

private:
	UPROPERTY()
	TObjectPtr<UDataTable> m_EnemyTable = nullptr;
	UPROPERTY()
	TSubclassOf<ABaseEnemy> m_EnemyDefault;
};
