// Fill out your copyright notice in the Description page of Project Settings.


#include "PPMonsterBase.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PPGame/Framework/PPCharacter.h"
#include "GameFramework/PlayerState.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"


APPMonsterBase::APPMonsterBase()
{
	bReplicates = true;
	
}

void APPMonsterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APPMonsterBase, Health, COND_SimulatedOnly);
}

void APPMonsterBase::MonsterTakeDamage(float InDamage, AActor* tInstigator)
{
	if (InDamage > 0.0f && bCanTakeDamage)
	{
		if (IsValid(tInstigator))
		{
			APPCharacter* tPlayer = Cast<APPCharacter>(tInstigator);
			if (IsValid(tPlayer))
			{
				Health -= InDamage;
				if (Health <= 0.0f)
				{
					Health = 0.0f;
					Dead(tInstigator);
					return;
				}

				if (!IsValid(Enemy))
				{
					SetEnemy(tPlayer);
				}
			}
		}
	}
}

void APPMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() == ROLE_Authority)
	{
		Health = HealthMax;
		SetMoveSpeed(MaxMoveSpeed);
	}
}

void APPMonsterBase::SetEnemy(APPCharacter* InEnemy)
{
	Enemy = InEnemy;

	AAIController* AIController = Cast<AAIController>(GetController());
	if (IsValid(AIController))
	{
		UBlackboardComponent* tBBComp = AIController->GetBlackboardComponent();
		if (IsValid(tBBComp))
		{
			tBBComp->SetValue<UBlackboardKeyType_Object>(TEXT("Enemy"), InEnemy);
		}
	}
}

void APPMonsterBase::Dead(AActor* tInstigator)
{
	bCanTakeDamage = false;

	// 设置行为树
	AAIController* AIController = Cast<AAIController>(GetController());
	if (IsValid(AIController))
	{
		UBlackboardComponent* tBBComp = AIController->GetBlackboardComponent();
		if (IsValid(tBBComp))
		{
			tBBComp->SetValue<UBlackboardKeyType_Bool>(TEXT("bDead"), true);
		}
	}
	
	Multi_CollisionDisable();
	GetCharacterMovement()->DisableMovement();
	Multi_PlayAnimMontage(DeadMontage);
	DetachFromControllerPendingDestroy();

	// 增加积分
	APPCharacter* tPlayer = Cast<APPCharacter>(tInstigator);
	if (IsValid(tPlayer))
	{
		AController* tController = tPlayer->GetController();
		if (IsValid(tController))
		{
			APlayerState* tPlayerState = tController->GetPlayerState<APlayerState>();
			if (IsValid(tPlayerState))
			{
				tPlayerState->SetScore(tPlayerState->GetScore() + 1);
			}
		}
	}
}

bool APPMonsterBase::MonsterIsAlive()
{
	return Health > 0.0f;
}

void APPMonsterBase::SetMoveSpeed(float Speed)
{
	MoveSpeed = Speed;
	Cast<UCharacterMovementComponent>(GetMovementComponent())->MaxWalkSpeed = Speed;
}

void APPMonsterBase::Multi_PlayAnimMontage_Implementation(UAnimMontage* Montage)
{
	if (IsValid(Montage))
	{
		PlayAnimMontage(Montage);	
	}
}

void APPMonsterBase::Multi_CollisionDisable_Implementation()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APPMonsterBase::Multi_PlayParticleSystem_Implementation(UParticleSystem* ParticleSystem, FVector Location)
{
	if (!IsNetMode(NM_DedicatedServer) && IsValid(ParticleSystem))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, Location);
	}
}

void APPMonsterBase::Multi_PlayDecal_Implementation(UMaterialInterface* DecalMat, FVector Size, FVector Location, float Life)
{
	if (!IsNetMode(NM_DedicatedServer) && IsValid(DecalMat))
	{
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), DecalMat, Size, Location, { -90.0f, 0.0f, 0.0f }, Life);
	}
}

void APPMonsterBase::SetDamageDeBuff(EPPDamageType DamageType, AActor* tInstigator)
{
	Multi_SetDamageDeBuff(DamageType);

	switch (DamageType)
	{
	case EPPDamageType::Normal:
		{
			
			break;
		}
	case EPPDamageType::Electric:
	{
		SetMoveSpeed(MaxMoveSpeed / 2.0f);
		GetWorldTimerManager().ClearTimer(ElectricHandle);
		GetWorldTimerManager().SetTimer(ElectricHandle, FTimerDelegate::CreateLambda([this]() {
			SetMoveSpeed(MaxMoveSpeed);
			}), 1.0f, false);
		break;
	}
	case EPPDamageType::Poison:
	{
		GetWorldTimerManager().ClearTimer(PoisonHandle);
		GetWorldTimerManager().SetTimer(PoisonHandle, FTimerDelegate::CreateLambda([this, tInstigator]() {
			MonsterTakeDamage(10.0f, tInstigator);
			}), 0.5f, true);
		GetWorldTimerManager().ClearTimer(PoisonStopHandle);
		GetWorldTimerManager().SetTimer(PoisonStopHandle, FTimerDelegate::CreateLambda([this]() {
			GetWorldTimerManager().ClearTimer(PoisonHandle);
			}), 2.0f, false);
		break;
	}
	case EPPDamageType::Fire:
	{
		GetWorldTimerManager().ClearTimer(Firehandle);
		GetWorldTimerManager().SetTimer(Firehandle, FTimerDelegate::CreateLambda([this, tInstigator]() {
			MonsterTakeDamage(10.0f, tInstigator);
			}), 0.5f, true);
		GetWorldTimerManager().ClearTimer(FireStopHandle);
		GetWorldTimerManager().SetTimer(FireStopHandle, FTimerDelegate::CreateLambda([this]() {
			GetWorldTimerManager().ClearTimer(Firehandle);
			}), 2.0f, false);
		break;
	}
	default:
		break;
	}
}

void APPMonsterBase::Multi_SetDamageDeBuff_Implementation(EPPDamageType DamageType)
{
	if (!IsNetMode(NM_DedicatedServer))
	{
		if (DeBuffEffects.Contains(DamageType) && IsValid(DeBuffEffects[DamageType]))
		{
			UParticleSystemComponent* tNewPs = UGameplayStatics::SpawnEmitterAttached(DeBuffEffects[DamageType], GetMesh(), DeBuffSocket);
			if (DeBuffPSMap.Contains(DamageType))
			{
				UParticleSystemComponent* tOldPS = DeBuffPSMap[DamageType];
				if (IsValid(tOldPS))
				{
					if (tOldPS->IsActive())
					{
						tOldPS->Deactivate();
					}
					tOldPS->DestroyComponent();
				}
				DeBuffPSMap[DamageType] = tNewPs;
			}
			else
			{
				DeBuffPSMap.Add(DamageType, tNewPs);
			}
		}
	}
}

UBlackboardComponent* APPMonsterBase::GetBlackboardComp()
{
	AAIController* tAIController = Cast<AAIController>(GetController());
	if (IsValid(tAIController))
	{
		return tAIController->GetBlackboardComponent();
	}

	return nullptr;
}
