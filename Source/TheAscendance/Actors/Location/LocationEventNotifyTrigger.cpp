// Fill out your copyright notice in the Description page of Project Settings.


#include "LocationEventNotifyTrigger.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "TheAscendance/Game/Subsystems/GameEventSubsystem.h"
#include "TheAscendance/Characters/Player/PlayerCharacter.h"

#include "Components/BoxComponent.h"

// Sets default values
ALocationEventNotifyTrigger::ALocationEventNotifyTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	checkf(m_Collider, TEXT("LocationEventNotifyTrigger failed to initialise m_Collider"));
}

void ALocationEventNotifyTrigger::BeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if(otherActor == nullptr || otherActor == this || (otherActor->IsA(APlayerCharacter::StaticClass()) == false))
	{
		return;
	}

	if (UGameEventSubsystem* gameEvent = UCoreFunctionLibrary::GetGameEventSubsystem())
	{
		gameEvent->NotifyLocationEnterred(LocationTag);
		m_Collider->OnComponentBeginOverlap.RemoveAll(this);
		Destroy();
	}
}

// Called when the game starts or when spawned
void ALocationEventNotifyTrigger::BeginPlay()
{
	Super::BeginPlay();

	//Eventually add conditions to determine if active. Such as if a specific quest is active. 
	m_Collider->OnComponentBeginOverlap.AddDynamic(this, &ALocationEventNotifyTrigger::BeginOverlap);
}

