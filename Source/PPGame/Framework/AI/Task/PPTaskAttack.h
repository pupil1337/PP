// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "PPTaskAttack.generated.h"

/**
 * 
 */
UCLASS()
class PPGAME_API UPPTaskAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPPTaskAttack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY()
	UAnimMontage* Montage = nullptr;
	UPROPERTY()
	UAnimInstance* AnimInstance = nullptr;
};
