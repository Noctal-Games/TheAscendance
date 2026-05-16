// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "PlayerMovementComponent.h"
#include "TheAscendance/Characters/Components/CharacterStatsComponent.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"
#include "TheAscendance/Characters/CharacterGameplayTags.h"
#include "TheAscendance/Game/Subsystems/AudioManagerSubsystem.h"
#include "TheAscendance/Characters/Components/LoadoutComponent.h"
#include "TheAscendance/Game/Subsystems/GameEventSubsystem.h"
#include "TheAscendance/Abilities/Spells/SpellGameplayTags.h"
#include "TheAscendance/Items/ItemGameplayTags.h"
#include "TheAscendance/Actors/Interaction/Interfaces/Interactable.h"
#include "TheAscendance/Characters/Components/EquipmentManagerComponent.h"


#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter() : ABaseCharacter()
{
	m_EquipmentManagerComponent = CreateDefaultSubobject<UEquipmentManagerComponent>(TEXT("Equipment Manager Component"));
	checkf(m_EquipmentManagerComponent, TEXT("Equipment Manager Component failed to initialise"));

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

EIdleType APlayerCharacter::GetIdleType()
{
	if (m_EquipmentManagerComponent == nullptr)
	{
		LOG_INFO("[PLAYER CHARACTER] EquipmentManager is invalid");
		return EIdleType::NO_ITEMS;
	}

	return m_EquipmentManagerComponent->GetIdleType();
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

bool APlayerCharacter::IsHoldingTwoHandedItem()
{
	if (m_EquipmentManagerComponent == nullptr)
	{
		LOG_INFO("[PLAYER CHARACTER] EquipmentManager is invalid");
		return false;
	}

	return m_EquipmentManagerComponent->IsHoldingTwoHandedItem();
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

UAbilityComponent* APlayerCharacter::GetAbilityComponent()
{
	return m_AbilityComponent;
}

float APlayerCharacter::PlayAnimationMontage(UAnimMontage* montageToPlay, float playRate, FName startSection)
{
	if (m_HandsMesh == nullptr)
	{
		LOG_ERROR("[PLAYER CHARACTER] Tried to play AnimationMontage but HandsMesh was invalid");
		return 0.0f;
	}

	if (montageToPlay == nullptr)
	{
		LOG_ERROR("Tried to play invalid animation montage");
		return 0.0f;
	}

	UAnimInstance* animInstance = m_HandsMesh->GetAnimInstance();

	if (animInstance == nullptr)
	{
		LOG_ERROR("Tried to play animation montage with invalid anim instance");
		return 0.0f;
	}

	float duration = animInstance->Montage_Play(montageToPlay, playRate);

	if (startSection.IsNone() == false)
	{
		animInstance->Montage_JumpToSection(startSection, montageToPlay);
	}

	return duration;
}

bool APlayerCharacter::EquipItem(EEquippablePart part, const FGameplayTag& itemTag)
{
	if (m_EquipmentManagerComponent == nullptr)
	{
		LOG_ERROR("[PLAYER CHARACTER] Tried to Equip item but EquipmentManagerComponent was null");
		return false;
	}

	return m_EquipmentManagerComponent->EquipItem(itemTag, part);
}

void APlayerCharacter::UnEquipItem(EEquippablePart part)
{
	if (m_EquipmentManagerComponent == nullptr)
	{
		LOG_ERROR("[PLAYER CHARACTER] Tried to Equip item but EquipmentManagerComponent was null");
		return;
	}

	m_EquipmentManagerComponent->UnEquipItem(part);
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

	TArray<USkeletalMeshComponent*> meshes;
	GetComponents<USkeletalMeshComponent>(meshes);

	for (USkeletalMeshComponent* mesh : meshes)
	{
		if (mesh == GetMesh())
		{
			continue;
		}

		m_HandsMesh = mesh;
	}

	if (m_EquipmentManagerComponent != nullptr)
	{
		m_EquipmentManagerComponent->Init(this, m_AbilityComponent, m_LoadoutComponent);
	}

	if(m_LoadoutComponent != nullptr)
	{
		m_LoadoutComponent->SetSpells(TestSpellTags);
	}
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

	HandleLookAtInteractions();
}

void APlayerCharacter::TestFunction1()
{
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
	LOG_ONSCREEN(-1, 1.0f, FColor::Yellow, "[PLAYER CHARACTER] TEST 3");

	if (m_TestEquipToggle == false)
	{
		if (EquipItem(EEquippablePart::RIGHT_HAND, ITEM_EQUIPMENT_GREATSWORD) == false)
		{
			return;
		}
	}
	else
	{
		UnEquipItem(EEquippablePart::RIGHT_HAND);
	}

	m_TestEquipToggle = !m_TestEquipToggle;
}

void APlayerCharacter::TestFunction3()
{
	LOG_ONSCREEN(-1, 1.0f, FColor::Yellow, "[PLAYER CHARACTER] TEST 3");

	if (m_TestEquipToggle2 == false)
	{
		if (EquipItem(EEquippablePart::LEFT_HAND, ITEM_EQUIPMENT_SWORD) == false)
		{
			return;
		}
	}
	else
	{
		UnEquipItem(EEquippablePart::LEFT_HAND);
	}

	m_TestEquipToggle2 = !m_TestEquipToggle2;
}

FVector APlayerCharacter::GetSocketLocationFromPart(EEquippablePart part)
{
	if(part != EEquippablePart::RIGHT_HAND && part != EEquippablePart::LEFT_HAND)
	{
		return Super::GetSocketLocationFromPart(part);
	}

	if(m_HandsMesh == nullptr)
	{
		LOG_ERROR("[PLAYER CHARACTER] Tried to get socket location from hand part but HandsMesh was invalid");
		return FVector::Zero();
	}

	FName socketName = GetSocketNameFromPart(part);

	if (m_HandsMesh->DoesSocketExist(socketName) == false)
	{
		LOG_ERROR("[PLAYER CHARACTER] Tried to get socket location for socket that doesn't exist: %s", *socketName.ToString());
		return FVector::Zero();
	}

	return m_HandsMesh->GetSocketLocation(socketName);
}

USkeletalMeshComponent* APlayerCharacter::GetEquipmentMesh()
{
	return m_HandsMesh;
}

void APlayerCharacter::AttackInputRelease()
{
	if (m_AbilityComponent == nullptr)
	{
		return;
	}

	m_AbilityComponent->OnInputReleased();
}

void APlayerCharacter::TestSetSpells(const TMap<EAbilitySlot, FGameplayTag>& spellTags)
{
	m_LoadoutComponent->SetSpells(spellTags);
}

