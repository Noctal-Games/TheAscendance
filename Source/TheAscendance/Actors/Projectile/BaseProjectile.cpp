// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseProjectile.h"
#include "TheAscendance/Core/CoreMacros.h"
#include "TheAscendance/Spells/Interfaces/Spell.h"
#include "TheAscendance/Spells/Structs/SpellData.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	checkf(m_Collider, TEXT("BaseProjectile Collider failed to initialise"));
	m_Collider->InitSphereRadius(50.0f);
	m_Collider->SetCollisionProfileName("Projectile");
	m_Collider->SetNotifyRigidBodyCollision(true);
	m_Collider->SetVisibility(true);
	m_Collider->bHiddenInGame = false;
	SetRootComponent(m_Collider);

	m_MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement Component"));
	checkf(m_MovementComponent, TEXT("BaseProjectile MovementComponent failed to initialise"));
	m_MovementComponent->UpdatedComponent = m_Collider;
	m_MovementComponent->bRotationFollowsVelocity = true;
	m_MovementComponent->bShouldBounce = false;

	m_StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	checkf(m_StaticMeshComponent, TEXT("BaseProjectile StaticMeshComponent failed to initialise"));

	//ConstructorHelpers::FObjectFinder<UStaticMesh>
	//mesh(TEXT("/Script/Engine.StaticMesh'/Game/LevelPrototyping/Meshes/SM_Cube.SM_Cube'"));

	//if (mesh.Succeeded())
	//{
	//	m_StaticMeshComponent->SetStaticMesh(mesh.Object);
	//}

	m_StaticMeshComponent->SetSimulatePhysics(false);
	m_StaticMeshComponent->SetVisibility(false);
	m_StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	m_StaticMeshComponent->SetupAttachment(m_Collider);
}

void ABaseProjectile::Init(ISpell* spell, UProjectileSpellData* spellData)
{
	if (spell == nullptr || spellData == nullptr)
	{
		LOG_ERROR("Tried to Init BaseProjectile with invalid Spell or SpellData");
		return;
	}

	m_Spell = spell->_getUObject();
	m_SpellData = spellData;

	float baseRadius = m_Collider->GetUnscaledSphereRadius();
	m_Collider->SetSphereRadius(baseRadius * m_SpellData->ProjectileSize);

	m_StartPos = GetActorLocation();
	m_MaxTravelDistance = m_SpellData->Range;
}

void ABaseProjectile::AddIgnoreActor(AActor* toIgnore)
{
	m_IgnoredOwner = toIgnore;
	m_Collider->IgnoreActorWhenMoving(toIgnore, true);
}

void ABaseProjectile::SetIsActive(bool isActive)
{
	m_IsActive = isActive;
	m_IsActive ? m_Collider->SetCollisionProfileName("Projectile") : m_Collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseProjectile::ApplyForce(FVector unitDirection)
{
	float speed = m_SpellData->ProjectileSpeed;

	m_MovementComponent->ProjectileGravityScale = m_SpellData->IsAffectedByGravity ? m_SpellData->GravityScale : 0.0f;

	m_MovementComponent->Velocity = unitDirection * speed;
	m_MovementComponent->InitialSpeed = speed;
	m_MovementComponent->MaxSpeed = speed;
}

void ABaseProjectile::SetDecoratedSelf(IProjectile* decoratedSelf)
{
	if (decoratedSelf == nullptr)
	{
		LOG_ERROR("Tried to set Projectile DecoratedSelf with invalid Projectile");
		return;
	}

	m_DecoratedSelf = decoratedSelf->_getUObject();
}

void ABaseProjectile::HandleOnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit)
{
	m_Spell->OnHit(otherActor, GetActorLocation());
	m_Spell->ProcessHit(GetActorLocation());
	
	Destroy();
}

void ABaseProjectile::HandleOnOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	HandleOnHit(overlappedComponent, otherActor, otherComp, FVector(), sweepResult);
}

void ABaseProjectile::HandleOnUpdate(float deltaTime)
{
	FVector distance = GetActorLocation() - m_StartPos;

	if (distance.Length() >= m_MaxTravelDistance)
	{
		Destroy();
	}
}

void ABaseProjectile::ProcessOverlapDamage(int& damage)
{
	if (m_Spell == nullptr)
	{
		return;
	}

	m_Spell->ProcessOverlapDamage(damage);
}

ISpell* ABaseProjectile::GetSpell()
{
	return m_Spell.GetInterface();
}

void ABaseProjectile::OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherComp, FVector normalImpulse, const FHitResult& hit)
{
	if (m_IsActive == false || otherActor == nullptr || otherActor == this || otherActor == m_IgnoredOwner)
	{
		return;
	}

	if (m_Spell == nullptr)
	{
		Destroy();
		return;
	}

	if (m_DecoratedSelf == nullptr)
	{
		LOG_ERROR("Projectile DecoratedSelf is invalid");
		HandleOnHit(hitComp, otherActor, otherComp, normalImpulse, hit);
		return;
	}

	m_DecoratedSelf->HandleOnHit(hitComp, otherActor, otherComp, normalImpulse, hit);
}

void ABaseProjectile::BeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	if (m_IsActive == false || otherActor == nullptr || otherActor == this || otherActor == m_IgnoredOwner)
	{
		return;
	}

	if (m_Spell == nullptr)
	{
		Destroy();
		return;
	}

	if (m_DecoratedSelf == nullptr)
	{
		LOG_ERROR("Projectile DecoratedSelf is invalid");
		HandleOnHit(overlappedComponent, otherActor, otherComp, FVector(), sweepResult);
		return;
	}

	m_DecoratedSelf->HandleOnOverlap(overlappedComponent, otherActor, otherComp, otherBodyIndex, bFromSweep, sweepResult);
	
    /*if (_canPenetrate == false || Cast<ACharacter>(otherActor) == nullptr)
	{
		_spell->ProcessHit(OtherActor, GetActorLocation(), 0);
		_spell = nullptr;
		Destroy();
		//ReturnToPool();

		return;
	}*/


	//m_Collider->IgnoreActorWhenMoving(otherActor, true);
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	m_Collider->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnHit);
	m_Collider->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::BeginOverlap);
}

// Called every frame
void ABaseProjectile::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (m_IsActive == false)
	{
		return;
	}

	if (m_DecoratedSelf == nullptr)
	{
		LOG_ERROR("Projectile DecoratedSelf is invalid");
		HandleOnUpdate(deltaTime);
		return;
	}

	HandleOnUpdate(deltaTime);
}

