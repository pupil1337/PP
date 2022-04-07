// Fill out your copyright notice in the Description page of Project Settings.


#include "PPBossSelectAttack.h"
#include "AIController.h"
#include "PPGame/Framework/AI/PPBossBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Int.h"


EBTNodeResult::Type UPPBossSelectAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APPBossBase* tBoss = Cast<APPBossBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (IsValid(tBoss))
	{
		const TArray<FMonsterSkill>& tSkills = tBoss->GetMonsterSkills();
		if (tSkills.Num() > 0)
		{
			TArray<int> SelectorArr;
			SelectorArr.Empty();
			for (int i = 0; i < tSkills.Num(); ++i)
			{
				for (int j = 0; j < tSkills[i].Weight; ++j)
				{
					SelectorArr.Add(i);
				}
			}
			if (SelectorArr.Num() > 0)
			{
				int Index = SelectorArr[UKismetMathLibrary::RandomInteger(SelectorArr.Num())];
				UBlackboardComponent* tBBComp = tBoss->GetBlackboardComp();
				if (IsValid(tBBComp))
				{
					tBBComp->SetValue<UBlackboardKeyType_Int>(TEXT("SkillIndex"), Index);
				}
			}		
		}
	}

	return EBTNodeResult::Failed;
}
