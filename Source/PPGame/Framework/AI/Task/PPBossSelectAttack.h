// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PPBossSelectAttack.generated.h"

/**
 * 
 */
UCLASS()
class PPGAME_API UPPBossSelectAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UPPBossSelectAttack()
	{
		NodeName = "Select Random Attack";
	}

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

};
