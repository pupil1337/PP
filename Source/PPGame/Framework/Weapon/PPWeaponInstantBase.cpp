// Fill out your copyright notice in the Description page of Project Settings.


#include "PPWeaponInstantBase.h"

#include "Kismet/GameplayStatics.h"

APPWeaponInstantBase::APPWeaponInstantBase()
{
}

void APPWeaponInstantBase::BeginPlay()
{
	Super::BeginPlay();
	
}

bool APPWeaponInstantBase::Fire()
{
	if (Super::Fire())
	{
		UKismetSystemLibrary::DrawDebugLine(this, CurrFireInfo.CameraLocation, CurrFireInfo.CameraRotation.Vector().Normalize() * FVector(1000.0f) + CurrFireInfo.CameraLocation, FLinearColor::Red, 0.5f);
	}
	return true;
}

void APPWeaponInstantBase::Aim(bool Op)
{
	Super::Aim(Op);
}
