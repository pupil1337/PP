// Copyright Epic Games, Inc. All Rights Reserved.


#include "PPCameraAnimInstance.h"


void UPPCameraAnimInstance::SetRotationMode(EPPRotationMode RotationMode)
{
	bVelocityDirection = RotationMode == EPPRotationMode::VelocityDirection;
	bLookingDirection = RotationMode == EPPRotationMode::LookingDirection;
	bAiming = RotationMode == EPPRotationMode::Aiming;
}
