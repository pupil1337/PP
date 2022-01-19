// Fill out your copyright notice in the Description page of Project Settings.


#include "PPWeaponInstantBase.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "PPGame/Framework/PPCharacter.h"

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
		FVector tStart = CurrFireInfo.CameraLocation;
		FVector tEnd   = tStart + CurrFireInfo.CameraRotation.Vector().Normalize() * 10000.0f;

		FCollisionQueryParams tParams;
		tParams.AddIgnoredActor(OwnerPawn);
		FHitResult tHitResult;
		bool bHit = GetWorld()->LineTraceSingleByChannel(tHitResult, tStart, tEnd, ECollisionChannel::ECC_Visibility, tParams);
		if (bHit)
		{
			tEnd = tHitResult.Location;
		}

		FVector tFireStart = GetMuzzleLocation();
		FRotator tRotation = (tEnd - tFireStart).Rotation();

		PlayTrailPS(tFireStart, tRotation);
	}
	return true;
}

void APPWeaponInstantBase::Aim(bool Op)
{
	Super::Aim(Op);
}

void APPWeaponInstantBase::PlayTrailPS(FVector Start, FRotator Rotation)
{
	if (IsValid(TrailPS))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TrailPS, Start, Rotation);

		if (OwnerPawn->GetLocalRole() == ROLE_AutonomousProxy)
		{
			Server_PlayTrailPS(Start, Rotation);
		}
		else if (GetNetMode() == NM_ListenServer)
		{
			Multicast_PlayTrailPS(Start, Rotation);
		}
	}
}

void APPWeaponInstantBase::Server_PlayTrailPS_Implementation(FVector Start, FRotator Rotation)
{
	Multicast_PlayTrailPS(Start, Rotation);
}

void APPWeaponInstantBase::Multicast_PlayTrailPS_Implementation(FVector Start, FRotator Rotation)
{
	if (OwnerPawn->GetLocalRole() == ROLE_SimulatedProxy)
	{
		PlayTrailPS(Start, Rotation);
	}
}
