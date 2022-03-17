// Fill out your copyright notice in the Description page of Project Settings.


#include "PPFindRandomPoint.h"
#include "PPGame/Framework/AI/PPMonsterBase.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "AIController.h"
#include "NavigationSystem.h"

EBTNodeResult::Type UPPFindRandomPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APPMonsterBase* tMonster = Cast<APPMonsterBase>(OwnerComp.GetAIOwner()->GetPawn());
	if (IsValid(tMonster))
	{
		FNavLocation ResultLocation;
		UNavigationSystemV1* NavSys = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		if (IsValid(NavSys))
		{
			if (NavSys->GetRandomReachablePointInRadius(tMonster->GetActorLocation(), Radius, ResultLocation))
			{
				UBlackboardComponent* tBBComp = tMonster->GetBlackboardComp();
				if (IsValid(tBBComp))
				{
					tBBComp->SetValue<UBlackboardKeyType_Vector>(BBLocationName, ResultLocation.Location);
					return EBTNodeResult::Succeeded;
				}
			}
		}
	}

	return EBTNodeResult::Failed;
}
