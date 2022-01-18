// Fill out your copyright notice in the Description page of Project Settings.


#include "PPWeaponInstantBase.h"

#include "Kismet/GameplayStatics.h"

APPWeaponInstantBase::APPWeaponInstantBase()
{
}

bool APPWeaponInstantBase::Fire()
{
	if (Super::Fire())
	{
		UKismetSystemLibrary::DrawDebugLine(this, CurrFireInfo.CameraLocation, CurrFireInfo.CameraRotation.Vector().Normalize() * FVector(1000.0f) + CurrFireInfo.CameraLocation, FLinearColor::Red, 0.5f);
	}
	return true;
}
