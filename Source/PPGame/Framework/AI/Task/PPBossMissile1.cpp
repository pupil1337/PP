// Fill out your copyright notice in the Description page of Project Settings.


#include "PPBossMissile1.h"
#include "AIController.h"
#include "PPGame/Framework/AI/PPBossBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"

EBTNodeResult::Type UPPBossMissile1::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AnimMontage = nullptr;
	AnimInstance = nullptr;

	APPBossBase* tBoss = Cast<APPBossBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (IsValid(tBoss))
	{
		UBlackboardComponent* tBBComp = tBoss->GetBlackboardComp();
		if (IsValid(tBBComp))
		{
			const TArray<FMonsterSkill>& tSkills = tBoss->GetMonsterSkills();
			int tIndex = tBBComp->GetValue<UBlackboardKeyType_Int>(TEXT("SkillIndex"));
			if (tIndex >= 0 && tIndex < tSkills.Num())
			{
				USkeletalMeshComponent* tMesh = tBoss->GetMesh();
				if (IsValid(tMesh))
				{
					UAnimInstance* tAnimInstance = tMesh->GetAnimInstance();
					if (IsValid(tAnimInstance) && IsValid(tSkills[tIndex].Montage))
					{
						tBoss->Multi_PlayAnimMontage(tSkills[tIndex].Montage);
						AnimInstance = tAnimInstance;
						AnimMontage = tSkills[tIndex].Montage;
						return EBTNodeResult::InProgress;
					}
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}

void UPPBossMissile1::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	if (IsValid(AnimInstance) && IsValid(AnimMontage))
	{
		if (AnimInstance->Montage_IsPlaying(AnimMontage) == false)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}
