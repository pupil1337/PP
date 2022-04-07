// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PPBossMissile1.generated.h"

class UAnimMontage;
class UAnimInstance;

/**
 * 
 */
UCLASS()
class PPGAME_API UPPBossMissile1 : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UPPBossMissile1()
	{
		NodeName = "Boss Missile 1";
		bNotifyTick = true;
	}

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY()
	UAnimMontage* AnimMontage = nullptr;

	UPROPERTY()
	UAnimInstance* AnimInstance = nullptr;

};
