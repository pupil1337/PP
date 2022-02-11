// Fill out your copyright notice in the Description page of Project Settings.


#include "PPMonsterBase.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "PPGame/Framework/PPCharacter.h"


APPMonsterBase::APPMonsterBase()
{
	bReplicates = true;
	
}

void APPMonsterBase::TakeDamage(float InDamage, AActor* Victim)
{
	if (InDamage > 0.0f)
	{
		if (IsValid(Victim))
		{
			APPCharacter* tPlayer = Cast<APPCharacter>(Victim);
			if (IsValid(tPlayer))
			{
				Health -= InDamage;
				if (Health <= 0.0f)
				{
					Health = 0.0f;
					Dead();
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

}

void APPMonsterBase::SetEnemy(APPCharacter* InEnemy)
{
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

void APPMonsterBase::Dead()
{
	
}

void APPMonsterBase::Multi_PlayAnimMontage_Implementation(UAnimMontage* Montage)
{
	if (IsValid(Montage))
	{
		PlayAnimMontage(Montage);	
	}
}
