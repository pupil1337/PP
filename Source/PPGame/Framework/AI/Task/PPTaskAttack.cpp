// Fill out your copyright notice in the Description page of Project Settings.


#include "PPTaskAttack.h"

#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "PPGame/Framework/AI/PPMonsterBase.h"


UPPTaskAttack::UPPTaskAttack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UPPTaskAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APPMonsterBase* tMonster = Cast<APPMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (IsValid(tMonster))
	{
		const TArray<FMonsterSkill>& tSkills = tMonster->GetMonsterSkills();
		if (tSkills.Num() > 0)
		{
			FMonsterSkill tSkill = tSkills[UKismetMathLibrary::RandomInteger(tSkills.Num())];
			if (IsValid(tSkill.Montage))
			{
				USkeletalMeshComponent* tMesh = tMonster->GetMesh();
				if (IsValid(tMesh))
				{
					AnimInstance = tMesh->GetAnimInstance();
					if (IsValid(AnimInstance))
					{
						tMonster->Multi_PlayAnimMontage(tSkill.Montage);
						Montage = tSkill.Montage;
						return EBTNodeResult::InProgress;
					}
				}
			}
		}
	}
	return EBTNodeResult::Failed;
}

void UPPTaskAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (IsValid(AnimInstance) && IsValid(Montage))
	{
		if (AnimInstance->Montage_IsPlaying(Montage) == false)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}