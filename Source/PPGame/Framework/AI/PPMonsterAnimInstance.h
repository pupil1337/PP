// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PPMonsterAnimInstance.generated.h"

class APPMonsterBase;

/**
 * 
 */
UCLASS()
class PPGAME_API UPPMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
protected:
	UPROPERTY(BlueprintReadOnly)
	float Speed;

private:
	UPROPERTY()
	APPMonsterBase* OwnerPawn = nullptr;
	
};
