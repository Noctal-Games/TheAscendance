// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "Components/CharacterStatsComponent.h"
#include "Components/LoadoutComponent.h"
#include "TheAscendance/Effects/Components/EffectHandlerComponent.h"
#include "TheAscendance/Items/HeldItem.h"
#include "TheAscendance/Game/GameModes/PlayableGameMode.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "CharacterTrajectoryComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_CharacterStatsComponent = CreateDefaultSubobject<UCharacterStatsComponent>(TEXT("Character Stats Component"));
	checkf(m_CharacterStatsComponent, TEXT("Character Stats Component failed to initialise"));
	m_EffectHandlerComponent = CreateDefaultSubobject<UEffectHandlerComponent>(TEXT("Effect Handler Component"));
	checkf(m_EffectHandlerComponent, TEXT("Effect Handler Component failed to initialise"));
	m_CharacterTrajectoryComponent = CreateDefaultSubobject<UCharacterTrajectoryComponent>(TEXT("Character Trajectory Component"));
	checkf(m_CharacterTrajectoryComponent, TEXT("Character Trajectory Component failed to initialise"));
	m_LoadoutComponent = CreateDefaultSubobject<ULoadoutComponent>(TEXT("Loadout Component"));
	checkf(m_LoadoutComponent, TEXT("Loadout Component failed to initialise"));

	SetRootComponent(GetCapsuleComponent());

	GetMesh()->SetCollisionProfileName(FName("NoCollision"));
	GetMesh()->SetupAttachment(GetRootComponent());
}

void ABaseCharacter::Heal(int amount)
{
	if (m_CharacterStatsComponent == nullptr)
	{
		LOG_ERROR("BaseCharacter has no CharacterStatsComponent");
		return;
	}

	m_CharacterStatsComponent->AdjustStatByValue(ECharacterStat::HEALTH, amount);
}

void ABaseCharacter::Damage(int amount)
{
	if (m_CharacterStatsComponent == nullptr)
	{
		LOG_ERROR("BaseCharacter has no CharacterStatsComponent");
		return;
	}

	m_CharacterStatsComponent->AdjustStatByValue(ECharacterStat::HEALTH, -amount);
}

void ABaseCharacter::ReduceStamina(int amount)
{
	if (m_CharacterStatsComponent == nullptr)
	{
		LOG_ERROR("BaseCharacter has no CharacterStatsComponent");
		return;
	}

	m_CharacterStatsComponent->AdjustStatByValue(ECharacterStat::STAMINA, -amount);
}

int ABaseCharacter::GetStat(ECharacterStat stat)
{
	if (m_CharacterStatsComponent == nullptr)
	{
		LOG_ERROR("BaseCharacter has no CharacterStatsComponent");
		return 0;
	}

	return m_CharacterStatsComponent->GetStatAsValue(stat);
}

bool ABaseCharacter::IsDead()
{
	return GetStat(ECharacterStat::HEALTH) <= 0.0f;
}

void ABaseCharacter::AddEffect(UCoreEffect* effect)
{
	if (m_EffectHandlerComponent == nullptr)
	{
		LOG_ERROR("BaseCharacter has no EffectHandlerComponent");
		return;
	}

	m_EffectHandlerComponent->AddEffect(effect);
}

void ABaseCharacter::AdjustStat(ECharacterStat stat, int amount)
{
	if (m_CharacterStatsComponent == nullptr)
	{
		LOG_ERROR("BaseCharacter has no CharacterStatsComponent");
		return;
	}

	switch (stat)
	{ 
		case ECharacterStat::WALK_SPEED:
		{
			m_CharacterStatsComponent->AdjustStatByPercentage(stat, amount);
			break;
		}
		case ECharacterStat::SPRINT_SPEED_BONUS:
		{
			m_CharacterStatsComponent->AdjustStatByPercentage(stat, amount);
			break;
		}
		case ECharacterStat::CROUCH_SPEED_PENALTY:
		{
			m_CharacterStatsComponent->AdjustStatByPercentage(stat, amount);
			break;
		}

		default:
		{
			m_CharacterStatsComponent->AdjustStatByValue(stat, amount);
		}
	}
}

void ABaseCharacter::AdjustMaxStat(ECharacterStat stat, int amount)
{
	if (m_CharacterStatsComponent == nullptr)
	{
		LOG_ERROR("BaseCharacter has no CharacterStatsComponent");
		return;
	}

	m_CharacterStatsComponent->AdjustMaxStatByValue(stat, amount);
}

AActor* ABaseCharacter::GetSusceptibleActor()
{
	return this;
}

void ABaseCharacter::AddImmunity(const FGameplayTag& immunity)
{
	m_EffectImmunities.AddTag(immunity);
}

void ABaseCharacter::AddResistance(const FGameplayTag& resistance)
{
	m_EffectResistances.AddTag(resistance);
}

bool ABaseCharacter::HasImmunity(const FGameplayTag& immunity) const
{
	return m_EffectImmunities.HasTag(immunity);
}

bool ABaseCharacter::HasResistance(const FGameplayTag& resistance) const
{
	return m_EffectResistances.HasTag(resistance);
}

bool ABaseCharacter::MainHandPrimaryAttack()
{
	if (IsAttacking() == true || m_MainHandItem == nullptr/* Can attack checks*/)
	{
		return false;
	}

	m_IsMainHandAttacking = m_MainHandItem->StartPrimaryAttack();

	if (m_IsMainHandAttacking == true)
	{
		m_AttackTimer = 0.5f;
	}

	return m_IsMainHandAttacking;
}

bool ABaseCharacter::MainHandSecondaryAttack()
{
	if (IsAttacking() == true || m_MainHandItem == nullptr/* Can attack checks*/)
	{
		return false;
	}

	m_IsMainHandAttacking = m_MainHandItem->StartSecondaryAttack();

	if (m_IsMainHandAttacking == true)
	{
		m_AttackTimer = 0.5f;
	}

	return m_IsMainHandAttacking;
}

bool ABaseCharacter::OffHandPrimaryAttack()
{
	if (IsAttacking() == true || m_OffHandItem == nullptr/* Can attack checks*/)
	{
		return false;
	}

	m_IsOffHandAttacking = m_OffHandItem->StartPrimaryAttack();

	if (m_IsOffHandAttacking == true)
	{
		m_AttackTimer = 0.5f;
	}

	return m_IsOffHandAttacking;
}

bool ABaseCharacter::OffHandSecondaryAttack()
{
	if (IsAttacking() == true || m_OffHandItem == nullptr/* Can attack checks*/)
	{
		return false;
	}

	m_IsOffHandAttacking = m_OffHandItem->StartSecondaryAttack();

	if (m_IsOffHandAttacking == true)
	{
		m_AttackTimer = 0.5f;
	}

	return m_IsOffHandAttacking;
}

bool ABaseCharacter::IsMainHandPrimaryAttacking()
{
	if (m_MainHandItem == nullptr)
	{
		return true;
	}

	return false;
}

bool ABaseCharacter::IsOffHandPrimaryAttacking()
{
	if (m_OffHandItem == nullptr)
	{
		return true;
	}

	return false;
}

EWeaponType ABaseCharacter::MainHandWeaponType()
{
	if (m_MainHandItem == nullptr)
	{
		return EWeaponType::HAND;
	}

	return m_MainHandItem->GetWeaponType();
}

EWeaponType ABaseCharacter::OffHandWeaponType()
{
	if (m_OffHandItem == nullptr)
	{
		return EWeaponType::HAND;
	}

	return m_OffHandItem->GetWeaponType();
}

void ABaseCharacter::EndMainHandAttack()
{
	if (m_MainHandItem == nullptr/* Can attack checks*/)
	{
		return;
	}

	m_IsMainHandAttacking = false;
	return m_MainHandItem->EndAttack();
}

void ABaseCharacter::EndOffHandAttack()
{
	if (m_OffHandItem == nullptr/* Can attack checks*/)
	{
		return;
	}

	m_IsOffHandAttacking = false;
	return m_OffHandItem->EndAttack();
}

AActor* ABaseCharacter::GetActor()
{
	return this;
}

const FVector ABaseCharacter::GetSpellOwnerLocation()
{
	return GetActorLocation();
}

const FVector ABaseCharacter::GetSpellOwnerForward()
{
	return GetActorForwardVector();
}

const FVector ABaseCharacter::GetCastStartLocation()
{
	return GetActorLocation();
}

const FVector ABaseCharacter::GetCastStartForward()
{
	return GetActorForwardVector();
}

void ABaseCharacter::GetOwnedGameplayTags(FGameplayTagContainer& tagContainer) const
{
	tagContainer.AppendTags(OwnedTags);
}

bool ABaseCharacter::HasMatchingGameplayTag(FGameplayTag tagToCheck) const
{
	return OwnedTags.HasTag(tagToCheck);
}

bool ABaseCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& tagContainer) const
{
	return OwnedTags.HasAll(tagContainer);
}

bool ABaseCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& tagContainer) const
{
	return OwnedTags.HasAny(tagContainer);
}

bool ABaseCharacter::IsSprinting()
{
	return m_IsSprinting;
}

void ABaseCharacter::SetDestination(const FVector& destination)
{
}

void ABaseCharacter::TurnTowards(const FRotator& targetRotation)
{
	m_TurnTargetRotation = targetRotation;
	m_IsTurning = true;
}

FVector ABaseCharacter::GetSocketLocation(FName socketName)
{
	if (GetMesh()->DoesSocketExist(socketName) == false)
	{
		return FVector::Zero();
	}

	return GetMesh()->GetSocketLocation(socketName);
}

float ABaseCharacter::PlayAnimationMontage(UAnimMontage* montageToPlay, float playRate, FName startSection)
{
	if (montageToPlay == nullptr)
	{
		LOG_ERROR("Tried to play invalid animation montage");
		return 0.0f;
	}

	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();

	if(animInstance == nullptr)
	{
		LOG_ERROR("Tried to play animation montage with invalid anim instance");
		return 0.0f;
	}

	float duration = animInstance->Montage_Play(montageToPlay, playRate);

	if (startSection.IsNone() == false)
	{
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(startSection, montageToPlay);
	}

	return duration;
}

bool ABaseCharacter::EquipItem(EEquippablePart part, int itemID)
{
	if(itemID <= 0)
	{
		UnEquipItem(part);
		return true;
	}

	//Remove from inventory logic

	switch (part)
	{
		case EEquippablePart::RIGHT_HAND:
		{
			if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
			{
				if (m_MainHandItem == nullptr)
				{
					LOG_ERROR("Main Hand Item is null");
					return false;
				}

				FItemData* itemData = gameMode->GetItemData(itemID);

				if (itemData == nullptr)
				{
					return false;
				}

				m_MainHandItem->Init(itemData);
				return true;
			}
			break;
		}

		case EEquippablePart::LEFT_HAND:
		{
			if (APlayableGameMode* gameMode = UCoreFunctionLibrary::GetPlayableGameMode())
			{
				if (m_OffHandItem == nullptr)
				{
					LOG_ERROR("OffHand Item is null");
					return false;
				}

				FItemData* itemData = gameMode->GetItemData(itemID);

				if (itemData == nullptr)
				{
					return false;
				}

				m_OffHandItem->Init(itemData);
				return true;
			}
			break;
		}

		default:
		{
			LOG_WARNING("EquipItem called with invalid EEquippablePart");
		}
	}

	return false;
}

void ABaseCharacter::UnEquipItem(EEquippablePart part)
{			
	//Add to inventory logic

	switch (part)
	{
		case EEquippablePart::RIGHT_HAND:
		{
			if (m_MainHandItem == nullptr)
			{
				LOG_ERROR("Main Hand Item is null");
				return;
			}

			m_MainHandItem->UnEquip();
			break;
		}

		case EEquippablePart::LEFT_HAND:
		{
			if (m_OffHandItem == nullptr)
			{
				LOG_ERROR("OffHand Item is null");
				return;
			}

			m_OffHandItem->UnEquip();
			break;
		}

		default:
		{
			LOG_WARNING("UnEquip called on invalid EquippablePart");
		}
	}
}

void ABaseCharacter::UpdateTurnTowards()
{
}

// Called every frame
void ABaseCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (m_AnimTest == false && IsAttacking() == true)
	{
		m_AttackTimer -= deltaTime;

		if (m_AttackTimer > 0)
		{
			return;
		}

		if (m_IsMainHandAttacking == true)
		{
			EndMainHandAttack();
		}
		else
		{
			EndOffHandAttack();
		}
	}

	if (m_IsTurning == true)
	{
		FRotator rotation = GetActorRotation();

		float deltaYaw = FMath::FindDeltaAngleDegrees(rotation.Yaw, m_TurnTargetRotation.Yaw);
		float maxYawThisFrame = 180.f * deltaTime;

		rotation.Yaw += FMath::Clamp(deltaYaw, -maxYawThisFrame, maxYawThisFrame);
		SetActorRotation(rotation);

		if (FMath::IsNearlyZero(deltaYaw, 0.5f))
		{
			rotation.Yaw = m_TurnTargetRotation.Yaw;
			SetActorRotation(rotation);
			m_IsTurning = false;
		}
	}
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	m_EffectHandlerComponent->Init(this);

	if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	{
		if (m_MainHandItem = world->SpawnActor<AHeldItem>(AHeldItem::StaticClass()))
		{
			m_MainHandItem->SetItemOwner(this);
			m_MainHandItem->UnEquip();

			m_MainHandItem->SetActorLocation(GetMesh()->GetSocketLocation("WeaponSocket_r"));
			m_MainHandItem->K2_AttachToComponent(GetMesh(), "WeaponSocket_r", EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);
		}

		if (m_OffHandItem = world->SpawnActor<AHeldItem>(AHeldItem::StaticClass()))
		{
			m_OffHandItem->SetItemOwner(this);
			m_OffHandItem->UnEquip();

			m_OffHandItem->SetActorLocation(GetMesh()->GetSocketLocation("WeaponSocket_l"));
			m_OffHandItem->K2_AttachToComponent(GetMesh(), "WeaponSocket_l", EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);
		}
	}

	m_CharacterStatsComponent->OnSpeedChanged.BindLambda([this](float walkSpeed) { GetCharacterMovement()->MaxWalkSpeed = walkSpeed; });
	m_CharacterStatsComponent->Init();

	//Test
	m_TestEquipToggle = false;
}