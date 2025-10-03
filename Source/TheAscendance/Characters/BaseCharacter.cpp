// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Core/CoreFunctionLibrary.h"
#include "Components/CharacterStatsComponent.h"
#include "TheAscendance/Items/HeldItem.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_CharacterStatsComponent = CreateDefaultSubobject<UCharacterStatsComponent>(TEXT("Character Stats Component"));
	checkf(m_CharacterStatsComponent, TEXT("Character Stats Component failed to initialise"));

	SetRootComponent(GetCapsuleComponent());

	GetMesh()->SetCollisionProfileName(FName("NoCollision"));
	GetMesh()->SetupAttachment(GetRootComponent());
}

void ABaseCharacter::Heal(int amount)
{
	if (m_CharacterStatsComponent == nullptr)
	{
		return;
	}

	m_CharacterStatsComponent->AdjustStatByValue(ECharacterStat::HEALTH, amount);
}

void ABaseCharacter::Damage(int amount)
{
	if (m_CharacterStatsComponent == nullptr)
	{
		return;
	}

	LOG_ONSCREEN(-1, 1.0f, FColor::Yellow, "%s took %i damage", *GetFName().ToString(), amount);
	m_CharacterStatsComponent->AdjustStatByValue(ECharacterStat::HEALTH, -amount);
}

void ABaseCharacter::ReduceStamina(int amount)
{
	if (m_CharacterStatsComponent == nullptr)
	{
		return;
	}

	m_CharacterStatsComponent->AdjustStatByValue(ECharacterStat::STAMINA, -amount);
}

int ABaseCharacter::GetStat(ECharacterStat stat)
{
	if (m_CharacterStatsComponent == nullptr)
	{
		return 0;
	}

	return m_CharacterStatsComponent->GetStatAsValue(stat);
}

bool ABaseCharacter::IsDead()
{
	return GetStat(ECharacterStat::HEALTH) <= 0.0f;
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

AActor* ABaseCharacter::GetSpellOwner()
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

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_AnimTest == false && IsAttacking() == true)
	{
		m_AttackTimer -= DeltaTime;

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
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

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



