// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PPBossAnimInstance.generated.h"

class APPBossBase;

/**
 * 
 */
UCLASS()
class PPGAME_API UPPBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation();
	virtual void NativeUpdateAnimation(float DeltaSeconds);

protected:
	UPROPERTY(BlueprintReadOnly)
	float fRunSpeed;
	UPROPERTY(BlueprintReadOnly)
	FVector MoveDir;

private:
	float RefreshTime = 0.0f;
	FVector LastPos;

	UPROPERTY()
	APPBossBase* OwnerPawn = nullptr;
};
