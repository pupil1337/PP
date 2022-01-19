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

bool APPWeaponInstantBase::Fire(bool Op)
{
	if (Super::Fire(Op))
	{
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), CurrFireInfo.CameraLocation, CurrFireInfo.CameraForward.Normalize() * FVector(100.0f) + CurrFireInfo.CameraLocation, FLinearColor::Red, 0.5f);
	}
	return true;
}

void APPWeaponInstantBase::Aim(bool Op)
{
	Super::Aim(Op);
}
