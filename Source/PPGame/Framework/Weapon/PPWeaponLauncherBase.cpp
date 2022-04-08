// Fill out your copyright notice in the Description page of Project Settings.


#include "PPWeaponLauncherBase.h"

#include "PPGame/Framework/Projectile/PPProjectileBase.h"
#include "PPGame/Framework/PPCharacter.h"

bool APPWeaponLauncherBase::Fire(bool Op)
{
	if (Super::Fire(Op))
	{
		FTraceResult tHit;
		CalcTraceResult(tHit);
		Server_SpawnProjectile(tHit.FireLocation, tHit.FireRotation);
	}
	return false;
}

void APPWeaponLauncherBase::Server_SpawnProjectile_Implementation(FVector Location, FRotator Rotation)
{
	FActorSpawnParameters tParams;
	tParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	tParams.Owner = OwnerPawn;
	GetWorld()->SpawnActor<APPProjectileBase>(BulletClass.Get(), Location, Rotation, tParams);
}
