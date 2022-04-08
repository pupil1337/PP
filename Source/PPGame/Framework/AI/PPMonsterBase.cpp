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


APPMonsterBase::APPMonsterBase()
{
	bReplicates = true;
	
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

UBlackboardComponent* APPMonsterBase::GetBlackboardComp()
{
	AAIController* tAIController = Cast<AAIController>(GetController());
	if (IsValid(tAIController))
	{
		return tAIController->GetBlackboardComponent();
	}

	return nullptr;
}
