// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "LocationEventNotifyTrigger.generated.h"

class UBoxComponent;

UCLASS()
class THEASCENDANCE_API ALocationEventNotifyTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALocationEventNotifyTrigger();

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Categories = "Location.Area"))
	FGameplayTag LocationTag;

private:
	UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Trigger Box"))
	TObjectPtr<UBoxComponent> m_Collider = nullptr;
};
