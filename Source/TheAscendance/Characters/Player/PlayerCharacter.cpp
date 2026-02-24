// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "PlayerMovementComponent.h"
#include "TheAscendance/Characters/Components/CharacterStatsComponent.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Items/HeldItem.h"
#include "TheAscendance/Spells/Interfaces/Spell.h"
#include "TheAscendance/Characters/CharacterGameplayTags.h"
#include "TheAscendance/Game/Subsystems/AudioManagerSubsystem.h"
#include "TheAscendance/Characters/Components/LoadoutComponent.h"
#include "TheAscendance/Game/Subsystems/GameEventSubsystem.h"
#include "TheAscendance/Spells/SpellGameplayTags.h"
#include "TheAscendance/Spells/Components/SpellCasterComponent.h"
#include "TheAscendance/Items/ItemGameplayTags.h"
#include "TheAscendance/Actors/Interaction/Interfaces/Interactable.h"

#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter() : ABaseCharacter()
{
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;

	m_Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	checkf(m_Camera, TEXT("Player Camera failed to initialise"));
	m_Camera->SetupAttachment(GetMesh(), TEXT("head"));
	m_Camera->bUsePawnControlRotation = true;
}

void APlayerCharacter::SetPlayerController(ACustomPlayerController* PlayerController)
{
	m_PlayerController = PlayerController;
}

ACustomPlayerController* APlayerCharacter::GetPlayerController()
{
	return m_PlayerController;
}

void APlayerCharacter::Interact()
{
	if (m_InteractTarget == nullptr)
	{
		LOG_INFO("[PLAYER CHARACTER] Interact Target is invalid");
		return;
	}

	m_InteractTarget->Interact(this);
}

bool APlayerCharacter::PickupItem(const FGameplayTag& itemTag, int amount)
{
	if (UGameEventSubsystem* gameEvent = UCoreFunctionLibrary::GetGameEventSubsystem())
	{
		gameEvent->NotifyItemPickup(itemTag, amount);
		return true;
	}

	return false;
}

void APlayerCharacter::SetIsSprinting(bool val)
{
	m_IsSprinting = val;
}
bool APlayerCharacter::IsSprinting()
{
	if (m_CharacterStatsComponent->GetStatAsValue(ECharacterStat::STAMINA) <= 0)
	{
		return false;
	}

	return m_IsSprinting;
}
void APlayerCharacter::SetIsCrouching(bool val)
{
	m_IsCrouching = val;
}
bool APlayerCharacter::IsCrouching()
{
	if (m_IsJumping == true)
	{
		return false;
	}

	return m_IsCrouching;
}
bool APlayerCharacter::IsCrouched()
{
	return m_CurrentCapsuleHeight < m_DefaultCapsuleHeight - 5;
}
void APlayerCharacter::SetIsJumping()
{
	m_IsJumping = true;
}
bool APlayerCharacter::CanJumpInternal_Implementation() const
{
	if (bIsCrouched == true || m_CharacterStatsComponent->GetStatAsValue(ECharacterStat::STAMINA) < 5)
	{
		return false;
	}

	if (JumpIsAllowedInternal() == false)
	{
		if (m_MovementComponent->IsSprinting() == true)
		{
			return true;
		}

		return false;
	}

	return true;
}
void APlayerCharacter::OnJumped_Implementation()
{
	Super::OnJumped_Implementation();

	m_CharacterStatsComponent->AdjustStatByValue(ECharacterStat::STAMINA, -5);
}
float APlayerCharacter::GetDefaultCapsuleHeight()
{
	return m_DefaultCapsuleHeight;
}
void APlayerCharacter::UpdateCrouchCamera(float deltaTime)
{
	float crouchSpeed = deltaTime * 5;

	if (m_MovementComponent->IsCrouchingCustom() && m_CurrentCapsuleHeight > m_CrouchCapsuleHeight)
	{
		m_CurrentCapsuleHeight = FMath::Lerp(m_CurrentCapsuleHeight, m_CrouchCapsuleHeight, crouchSpeed);
		GetCapsuleComponent()->SetCapsuleSize(m_DefaultCapsuleRadius, m_CurrentCapsuleHeight, true);
	}
	if (m_MovementComponent->IsCrouchingCustom() == false && m_CurrentCapsuleHeight < m_DefaultCapsuleHeight)
	{
		m_CurrentCapsuleHeight = FMath::Lerp(m_CurrentCapsuleHeight, m_DefaultCapsuleHeight, crouchSpeed);
		GetCapsuleComponent()->SetCapsuleSize(m_DefaultCapsuleRadius, m_CurrentCapsuleHeight, true);
	}
}
void APlayerCharacter::OnMovementModeChanged(EMovementMode prevMovementMode, uint8 previousCustomMode)
{
	Super::OnMovementModeChanged(prevMovementMode, previousCustomMode);

	if (prevMovementMode != EMovementMode::MOVE_Falling)
	{
		return;
	}

	m_IsJumping = false;
}

UCameraComponent* APlayerCharacter::GetCamera()
{
	return m_Camera;
}

const FVector APlayerCharacter::GetCastStartForward()
{
	return m_Camera->GetForwardVector();
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	ABaseCharacter::BeginPlay();

	OwnedTags.AddTag(CHARACTER_PLAYER);

	m_MovementComponent = Cast<UPlayerMovementComponent>(GetMovementComponent());
	checkf(m_MovementComponent, TEXT("Movement Component is an invalid value"));

	m_DefaultCapsuleRadius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	m_DefaultCapsuleHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight();
	m_CurrentCapsuleHeight = m_DefaultCapsuleHeight;
	m_CrouchCapsuleHeight = m_DefaultCapsuleHeight / 2;

	//Test
	TArray<FGameplayTag> testSpells;
	testSpells.SetNum(4);
	testSpells[2] = SPELL_FIRE_BOLT;

	m_LoadoutComponent->SetSpells(testSpells);
}

void APlayerCharacter::HandleLookAtInteractions()
{
	FVector loc;
	FRotator rot;
	FHitResult hit;

	AController* controller = GetController();

	if (controller == nullptr)
	{
		LOG_ERROR("[PLAYER CHARACTER] Controller was invalid")
			return;
	}

	controller->GetPlayerViewPoint(loc, rot);

	FVector start = loc;
	FVector end = start + (rot.Vector() * InteractRange);

	FCollisionQueryParams traceParams;
	traceParams.AddIgnoredActor(this);

	bool iHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, traceParams);

	CheckForInteractTarget(iHit, hit);
}

void APlayerCharacter::CheckForInteractTarget(bool iHit, const FHitResult& hit)
{
	if (iHit == true)
	{
		AActor* hitActor = hit.GetActor();

		if (m_InteractTarget.GetObject() == hitActor)
		{
			return;
		}

		if (IInteractable* target = Cast<IInteractable>(hitActor))
		{
			m_InteractTarget.SetObject(hitActor);
			m_InteractTarget.SetInterface(target);
		}
		else
		{
			m_InteractTarget = nullptr;
		}
	}
	else
	{
		m_InteractTarget = nullptr;
	}

	if (m_OnInteractTargetChanged.IsBound())
	{
		if (m_InteractTarget == nullptr)
		{
			m_OnInteractTargetChanged.Execute(EInteractType::MAX);
			return;
		}

		m_OnInteractTargetChanged.Execute(m_InteractTarget->GetInteractType());
	}
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	ABaseCharacter::Tick(DeltaTime);

	LOG_ONSCREEN(0, 1, FColor::Yellow, "[PLAYER CHARACTER] USING ANIMATIONS: %s", m_AnimTest ? TEXT("TRUE") : TEXT("FALSE"));

	HandleLookAtInteractions();
}

void APlayerCharacter::TestFunction1()
{
	LOG_ONSCREEN(-1, 1.0f, FColor::Yellow, "[PLAYER CHARACTER] TEST 1");
	m_AnimTest = !m_AnimTest;

	if (m_TestSound != nullptr)
	{
		if(UWorld* world = UCoreFunctionLibrary::GetGameWorld())
		{
			if (UAudioManagerSubsystem* audioManager = world->GetGameInstance()->GetSubsystem<UAudioManagerSubsystem>())
			{
				audioManager->PlaySoundAtLocation(m_TestSound, GetActorLocation(), 1.0f, true);
			}
		}
	}
}

void APlayerCharacter::TestFunction2()
{
	//LOG_ONSCREEN(-1, 1.0f, FColor::Yellow, "TEST 2");
	//EndMainHandAttack();
	//EndOffHandAttack();

}

void APlayerCharacter::TestFunction3()
{
	LOG_ONSCREEN(-1, 1.0f, FColor::Yellow, "[PLAYER CHARACTER] TEST 3");

	if (m_TestEquipToggle == false)
	{
		m_LoadoutComponent->EquipItem(EEquippablePart::LEFT_HAND, ITEM_EQUIPMENT_SWORD);
	}
	else
	{
		m_LoadoutComponent->UnEquipItem(EEquippablePart::LEFT_HAND);
	}

	m_TestEquipToggle = !m_TestEquipToggle;

	float rand = FMath::RandRange(0, 100);

	if (rand < 50)
	{
		m_CharacterStatsComponent->AdjustStatByValue(ECharacterStat::SHIELD, 20);
	}
	else
	{
		m_CharacterStatsComponent->AdjustStatByValue(ECharacterStat::SHIELD, -10);
	}
}

