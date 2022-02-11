// Fill out your copyright notice in the Description page of Project Settings.


#include "PPMonsterAnimInstance.h"

#include "PPMonsterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPPMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerPawn = Cast<APPMonsterBase>(TryGetPawnOwner());
}

void UPPMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (IsValid(OwnerPawn))
	{
		Speed = OwnerPawn->GetCharacterMovement()->Velocity.Size();
	}
}
