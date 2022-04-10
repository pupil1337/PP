// Fill out your copyright notice in the Description page of Project Settings.


#include "PPProjectileBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PPGame/Framework/PPCharacter.h"
#include "PPGame/Framework/AI/PPMonsterBase.h"
#include "PPGame/Framework/Component/PPAttributeComp.h"

APPProjectileBase::APPProjectileBase()
{
	// collision component defaults.
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CollisionComponent->SetCapsuleSize(15.0f, 15.0f);
	RootComponent = CollisionComponent;

	// movement component defaults.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->InitialSpeed = 5000.f;
	ProjectileMovementComponent->MaxSpeed = 10000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	FlyParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FlyParticleComponent"));
	FlyParticleComponent->bAutoActivate = true;
	FlyParticleComponent->bAutoDestroy = true;
	FlyParticleComponent->SetupAttachment(RootComponent);

	InitialLifeSpan = 10.0f;
	
	bReplicates = true;
	SetReplicatingMovement(true);
}

void APPProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovementComponent->OnProjectileStop.AddUniqueDynamic(this, &APPProjectileBase::OnProjectileStop);
}

void APPProjectileBase::OnProjectileStop(const FHitResult& ImpactResult)
{
	if (GetNetMode() != NM_DedicatedServer)
	{
		// 子弹特效
		if (IsValid(FlyParticleComponent))
		{
			FlyParticleComponent->DeactivateSystem();
			FlyParticleComponent->DestroyComponent();
		}
		
		// 爆炸特效
		if (IsValid(ExplodParticleSystem))
		{
			ExplodParticleComponent = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodParticleSystem, GetActorLocation());
		}
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		// 打到怪
		TArray<AActor*> tFoundMonsterArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APPMonsterBase::StaticClass(), tFoundMonsterArray);
		ExplodMonsterArray.Empty();
		for (auto& it : tFoundMonsterArray)
		{
			APPMonsterBase* tMonster = Cast<APPMonsterBase>(it);
			if (IsValid(tMonster))
			{
				if (FVector::Distance(tMonster->GetActorLocation(), GetActorLocation()) < ExplodRadius)
				{
					tMonster->MonsterTakeDamage(ExplodDamage, GetOwner());
					ExplodMonsterArray.Add(it);
				}
			}
		}

		// 打到玩家
		TArray<AActor*> tFoundPlayerArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APPCharacter::StaticClass(), tFoundPlayerArray);
		ExplodPlayerArray.Empty();
		for (auto& it : tFoundPlayerArray)
		{
			APPCharacter* tPlayer = Cast<APPCharacter>(it);
			if (IsValid(tPlayer))
			{
				if (FVector::Distance(tPlayer->GetActorLocation(), GetActorLocation()) < ExplodRadius)
				{
					UPPAttributeComp* tComp = Cast<UPPAttributeComp>(tPlayer->GetComponentByClass(UPPAttributeComp::StaticClass()));
					if (IsValid(tComp))
					{
						tComp->TakeDamage(GetOwner(), 10.0f);
						ExplodPlayerArray.Add(it);
					}
				}
			}
		}
	}

	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(1.0f);
}
