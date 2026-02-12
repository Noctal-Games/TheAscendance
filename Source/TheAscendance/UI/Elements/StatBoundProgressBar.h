// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ProgressBar.h"
#include "TheAscendance/Characters/Enums/CharacterStat.h"
#include "StatBoundProgressBar.generated.h"

class UCharacterStatsComponent;

UENUM(BlueprintType)
enum class EHUDBarStat : uint8
{
	NONE UMETA(DisplayName = "None"),
	HEALTH UMETA(DisplayName = "Health"),
	STAMINA UMETA(DisplayName = "Stamina"),
	MANA UMETA(DisplayName = "Mana"),
	SHIELD UMETA(DisplayName = "Shield")
};

UCLASS()
class THEASCENDANCE_API UStatBoundProgressBar : public UProgressBar
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Init(UCharacterStatsComponent* statsComponent);

protected:
	void BindToStat();
	void UnbindFromStat();
	void HandleStatChanged(ECharacterStat stat, float current, float max);

	virtual void OnWidgetRebuilt() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	ECharacterStat GetCharacterStatFromHUDStat(EHUDBarStat stat);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (DisplayName = "Bound Stat"))
	EHUDBarStat BoundHUDStat = EHUDBarStat::NONE;

private:
	ECharacterStat m_BoundStat = ECharacterStat::NONE;

	TWeakObjectPtr<UCharacterStatsComponent> m_OwnerStatsComponent = nullptr;

	FDelegateHandle m_BoundStatDelegateHandle;
};
