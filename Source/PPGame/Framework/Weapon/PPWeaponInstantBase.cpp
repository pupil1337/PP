// Fill out your copyright notice in the Description page of Project Settings.


#include "PPWeaponInstantBase.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "PPGame/Framework/PPCharacter.h"
#include "PPGame/Framework/AI/PPMonsterBase.h"
#include "PPGame/Framework/Component/PPAttributeComp.h"


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
		FTraceResult tHit;
		CalcTraceResult(tHit);
		PlayTrailPS(tHit.FireLocation, tHit.FireRotation);
		TakeDamageTo(tHit.HitActor);
	}
	return true;
}

void APPWeaponInstantBase::Aim(bool Op)
{
	Super::Aim(Op);
}

void APPWeaponInstantBase::TakeDamageTo(AActor* Victim)
{
	if (IsValid(Victim))
	{
		if (OwnerPawn->GetLocalRole() == ROLE_AutonomousProxy)
		{
			Server_TakeDamageTo(Victim);
		}
		else if (OwnerPawn->GetLocalRole() == ROLE_Authority)
		{
			// 1、打到玩家
			APPCharacter* tPlayer = Cast<APPCharacter>(Victim);
			if (IsValid(tPlayer))
			{
				UPPAttributeComp* tComp = Cast<UPPAttributeComp>(tPlayer->GetComponentByClass(UPPAttributeComp::StaticClass()));
				if (IsValid(tComp))
				{
					tComp->TakeDamage(OwnerPawn, WeaponCfg.Damage);
				}
			}

			// 2、打到怪物
			APPMonsterBase* tMonster = Cast<APPMonsterBase>(Victim);
			if (IsValid(tMonster))
			{
				tMonster->MonsterTakeDamage(WeaponCfg.Damage, OwnerPawn);
			}
		}
	}
}


void APPWeaponInstantBase::Server_TakeDamageTo_Implementation(AActor* Victim)
{
	TakeDamageTo(Victim);
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
	if (IsValid(OwnerPawn) && OwnerPawn->GetLocalRole() == ROLE_SimulatedProxy)
	{
		PlayTrailPS(Start, Rotation);
	}
}
