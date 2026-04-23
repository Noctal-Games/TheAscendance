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

void ABaseCharacter::Damage(int amount, bool triggerOnHit)
{
	if (m_CharacterStatsComponent == nullptr)
	{
		LOG_ERROR("BaseCharacter has no CharacterStatsComponent");
		return;
	}

	if (triggerOnHit == true)
	{
		OnHit.Broadcast();
	}

	m_CharacterStatsComponent->AdjustStatByValue(ECharacterStat::HEALTH, -amount);
	
	OnDamageTaken.Broadcast();

	if (m_CharacterStatsComponent->GetStatAsValue(ECharacterStat::HEALTH) <= 0.0f)
	{
		OnDeath.Broadcast(this);
	}
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

	//m_IsMainHandAttacking = m_MainHandItem->StartPrimaryAttack();

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

	//m_IsMainHandAttacking = m_MainHandItem->StartSecondaryAttack();

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

	//m_IsOffHandAttacking = m_OffHandItem->StartPrimaryAttack();

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

	//m_IsOffHandAttacking = m_OffHandItem->StartSecondaryAttack();

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

	return EWeaponType::HAND;
}

EWeaponType ABaseCharacter::OffHandWeaponType()
{
	if (m_OffHandItem == nullptr)
	{
		return EWeaponType::HAND;
	}

	return EWeaponType::HAND;
}

void ABaseCharacter::EndMainHandAttack()
{
	if (m_MainHandItem == nullptr/* Can attack checks*/)
	{
		return;
	}

	m_IsMainHandAttacking = false;
	return;
}

void ABaseCharacter::EndOffHandAttack()
{
	if (m_OffHandItem == nullptr/* Can attack checks*/)
	{
		return;
	}

	m_IsOffHandAttacking = false;
	return;
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

void ABaseCharacter::CastSpell(int slot)
{
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
		LOG_ERROR("[BASE CHARACTER] Tried to get socket location for socket that doesn't exist: %s", *socketName.ToString());
		return FVector::Zero();
	}

	return GetMesh()->GetSocketLocation(socketName);
}

FVector ABaseCharacter::GetSocketLocationFromPart(EEquippablePart part)
{
	return GetSocketLocation(GetSocketNameFromPart(part));
}

FName ABaseCharacter::GetSocketNameFromPart(EEquippablePart part)
{
	switch (part)
	{
	case EEquippablePart::NONE:
		break;
	case EEquippablePart::RIGHT_HAND:
		return "WeaponSocket_r";
	case EEquippablePart::LEFT_HAND:
		return "WeaponSocket_l";
	default:
		break;
	}

	LOG_ERROR("[BASE CHARACTER] Tried to get socket name for part but socket doesn't exist: %s", *UEnum::GetValueAsString(part));
	return FName();
}

USkeletalMeshComponent* ABaseCharacter::GetEquipmentMesh()
{
	return GetMesh();
}

float ABaseCharacter::PlayAnimationMontage(UAnimMontage* montageToPlay, float playRate, FName startSection)
{
	if (montageToPlay == nullptr) 
	{
		LOG_ERROR("[BASE CHARACTER] Tried to play invalid animation montage");
		return 0.0f;
	}

	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();

	if(animInstance == nullptr)
	{
		LOG_ERROR("[BASE CHARACTER] Tried to play animation montage with invalid anim instance");
		return 0.0f;
	}

	float duration = animInstance->Montage_Play(montageToPlay, playRate);

	if (startSection.IsNone() == false)
	{
		GetMesh()->GetAnimInstance()->Montage_JumpToSection(startSection, montageToPlay);
	}

	return duration;
}
void ABaseCharacter::TriggerAbility()
{
}

void ABaseCharacter::StopAbility()
{
}

bool ABaseCharacter::EquipItem(EEquippablePart part, const FGameplayTag& itemTag)
{
	if(m_LoadoutComponent == nullptr)
	{
		LOG_ERROR("[BASE CHARACTER] Tried to Equip item but LoadoutComponent was null");
		return false;
	}

	//Remove from Inventory

	m_LoadoutComponent->EquipItem(part, itemTag);
	return true;
}

void ABaseCharacter::UnEquipItem(EEquippablePart part)
{
	if (m_LoadoutComponent == nullptr)
	{
		LOG_ERROR("[BASE CHARACTER] Tried to UnEquip item but LoadoutComponent was null");
		return;
	}

	//Add to inventory logic

	m_LoadoutComponent->UnEquipItem(part);
}

UCharacterStatsComponent* ABaseCharacter::GetCharacterStatsComponent()
{
	return m_CharacterStatsComponent;
}

ULoadoutComponent* ABaseCharacter::GetLoadoutComponent() const
{
	return m_LoadoutComponent;
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

	//if (UWorld* world = UCoreFunctionLibrary::GetGameWorld())
	//{
	//	if (m_MainHandItem = world->SpawnActor<AHeldItem>(AHeldItem::StaticClass()))
	//	{
	//		m_MainHandItem->SetItemOwner(this);
	//		m_MainHandItem->UnEquip();

	//		m_MainHandItem->SetActorLocation(GetMesh()->GetSocketLocation("WeaponSocket_r"));
	//		m_MainHandItem->K2_AttachToComponent(GetMesh(), "WeaponSocket_r", EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);

	//		m_MainHandItem->m_IsOffHand = false;
	//	}

	//	if (m_OffHandItem = world->SpawnActor<AHeldItem>(AHeldItem::StaticClass()))
	//	{
	//		m_OffHandItem->SetItemOwner(this);
	//		m_OffHandItem->UnEquip();

	//		m_OffHandItem->SetActorLocation(GetMesh()->GetSocketLocation("WeaponSocket_l"));
	//		m_OffHandItem->K2_AttachToComponent(GetMesh(), "WeaponSocket_l", EAttachmentRule::SnapToTarget, EAttachmentRule::KeepRelative, EAttachmentRule::KeepRelative, true);

	//		m_MainHandItem->m_IsOffHand = true;
	//	}
	//}

	m_CharacterStatsComponent->OnStatChanged.AddLambda([this](ECharacterStat stat, float walkSpeed, float ignore) 
		{ 
			if(stat != ECharacterStat::WALK_SPEED)
			{
				return;
			}

			GetCharacterMovement()->MaxWalkSpeed = walkSpeed; 
		}
	);

	m_CharacterStatsComponent->Init();

	//Test
	m_TestEquipToggle = false;
}