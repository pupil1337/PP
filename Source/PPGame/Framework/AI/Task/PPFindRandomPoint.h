// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PPFindRandomPoint.generated.h"

/**
 * 
 */
UCLASS()
class PPGAME_API UPPFindRandomPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPPFindRandomPoint()
	{
		NodeName = "Find Random Point";
	}

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	
protected:
	UPROPERTY(EditAnywhere)
	float Radius = 1000.0f;
	UPROPERTY(EditAnywhere)
	FName BBLocationName;

};
