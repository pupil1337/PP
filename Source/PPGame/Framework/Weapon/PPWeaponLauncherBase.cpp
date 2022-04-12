// Fill out your copyright notice in the Description page of Project Settings.


#include "PPWeaponLauncherBase.h"

#include "PPGame/Framework/Projectile/PPProjectileBase.h"
#include "PPGame/Framework/PPCharacter.h"
#include "Kismet/KismetMathLibrary.h"

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

	bool bSuccess = false;
	for (auto& it: BulletClass)
	{
		if (UKismetMathLibrary::RandomFloat() <= it.pro)
		{
			GetWorld()->SpawnActor<APPProjectileBase>(it.BulletClass.Get(), Location, Rotation, tParams);
			bSuccess = true;
		}
	}
	if (!bSuccess)
	{
		const TSubclassOf<APPProjectileBase>& tSubClass = BulletClass[UKismetMathLibrary::RandomInteger(BulletClass.Num())].BulletClass;
		GetWorld()->SpawnActor<APPProjectileBase>(tSubClass.Get(), Location, Rotation, tParams);
	}
}

void APPWeaponLauncherBase::ProjectileUpPro(TSubclassOf<APPProjectileBase> InBulletClass)
{
	for (auto& it: BulletClass)
	{
		if (it.BulletClass == InBulletClass)
		{
			it.pro += 0.2f;
		}
	}
}
