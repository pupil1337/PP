// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPlayerAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
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
		PPAnimInfo.Velocity = OwnerPawn->GetCharacterMovement()->Velocity;
		UpdateMoveDir();
		
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
