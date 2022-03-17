// Fill out your copyright notice in the Description page of Project Settings.


#include "PPBossAnimInstance.h"

#include "PPBossBase.h"
#include "Kismet/KismetMathLibrary.h"


void UPPBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerPawn = Cast<APPBossBase>(TryGetPawnOwner());
}

void UPPBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(OwnerPawn))
	{
		fRunSpeed = OwnerPawn->GetVelocity().Size();

		// 更新Move方向
		RefreshTime += DeltaSeconds;
		if (RefreshTime > 0.2f)
		{
			FVector tCurrPos = OwnerPawn->GetActorLocation();
			FVector tMoveDir = tCurrPos - LastPos;
			tMoveDir = UKismetMathLibrary::InverseTransformDirection(OwnerPawn->GetActorTransform(), tMoveDir);
			tMoveDir.Normalize();
			tMoveDir *= 2.0f;

			MoveDir = tMoveDir;
			LastPos = tCurrPos;
			RefreshTime = 0.0f;
		}

	}
}
