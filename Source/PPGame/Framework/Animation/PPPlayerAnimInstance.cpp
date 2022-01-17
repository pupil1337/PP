// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPlayerAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PPGame/Framework/PPCharacter.h"

void UPPPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerPawn = Cast<APPCharacter>(TryGetPawnOwner());
}

void UPPPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(OwnerPawn) && DeltaSeconds > 0.0f)
	{
		if (!SimulatedProxyRefresh())
		{
			return;
		}
		
		PPAnimInfo.Velocity = OwnerPawn->GetCharacterMovement()->Velocity;
		if (PPAnimInfo.Velocity.Z > 1.0f || PPAnimInfo.Velocity.Z < -1.0f)
		{
			PPAnimInfo.MovementState = EPPMovementState::InAir;
		}
		else if (PPAnimInfo.MovementState == EPPMovementState::InAir)
		{
			PPAnimInfo.MovementState = EPPMovementState::Grounded;
		}
		
		UpdateMoveDir();
		PPAnimInfo.AimPitch = OwnerPawn->GetCameraAimPitch();
	}
}

void UPPPlayerAnimInstance::UpdateMoveDir()
{
	FVector tCurrPos = OwnerPawn->GetActorLocation();
	
	FVector tMoveDir = tCurrPos - LastPos;
	if (tMoveDir.IsNearlyZero())
	{
		return;
	}
	
	FVector tForwardDir = OwnerPawn->GetActorForwardVector();
	tForwardDir.Z = 0.0f;
	tForwardDir.Normalize();
	
	tMoveDir.Z = 0;
	tMoveDir = UKismetMathLibrary::InverseTransformDirection(OwnerPawn->GetActorTransform(), tMoveDir);
	tMoveDir.Normalize();
	
	float angle = UKismetMathLibrary::DegAcos(FVector::DotProduct(FVector::ForwardVector, tMoveDir));
	FVector tNormal = FVector::CrossProduct(FVector::ForwardVector, tMoveDir);
	PPAnimInfo.MoveDir = tNormal.Z < 0.0f ? -angle : angle;
	
	LastPos = tCurrPos;
}

bool UPPPlayerAnimInstance::SimulatedProxyRefresh()
{
	if (OwnerPawn->GetLocalRole() == ROLE_SimulatedProxy)
	{
		float tCurrTime = UGameplayStatics::GetTimeSeconds(this);
		if (tCurrTime - LastRefreshTime < 0.1f)
		{
			return false;
		}
		LastRefreshTime = tCurrTime;
	}
	return true;
}
