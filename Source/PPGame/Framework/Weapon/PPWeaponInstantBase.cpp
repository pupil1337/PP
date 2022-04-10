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
		PlayPS(TrailPS, tHit.FireLocation, tHit.FireRotation);
		TakeDamageTo(tHit.HitActor, tHit.HitLocation);
	}
	return true;
}

void APPWeaponInstantBase::Aim(bool Op)
{
	Super::Aim(Op);
}

void APPWeaponInstantBase::TakeDamageTo(AActor* Victim, FVector Location)
{
	if (IsValid(Victim))
	{
		APPCharacter* tPlayer = Cast<APPCharacter>(Victim);
		APPMonsterBase* tMonster = Cast<APPMonsterBase>(Victim);
		if (OwnerPawn->GetLocalRole() == ROLE_AutonomousProxy)
		{
			Server_TakeDamageTo(Victim, Location);
		}
		else if (OwnerPawn->GetLocalRole() == ROLE_Authority)
		{
			// 1、打到玩家
			if (IsValid(tPlayer))
			{
				UPPAttributeComp* tComp = Cast<UPPAttributeComp>(tPlayer->GetComponentByClass(UPPAttributeComp::StaticClass()));
				if (IsValid(tComp))
				{
					tComp->TakeDamage(OwnerPawn, WeaponCfg.Damage);
				}
			}

			// 2、打到怪物
			if (IsValid(tMonster))
			{
				tMonster->MonsterTakeDamage(WeaponCfg.Damage, OwnerPawn);
			}
		}

		if (OwnerPawn->IsLocallyControlled())
		{
			// 1、打到玩家
			if (IsValid(tPlayer))
			{
				PlayPS(HitPlayerPS, Location, FRotator::ZeroRotator);
			}

			// 2、打到怪物
			if (IsValid(tMonster))
			{
				PlayPS(HitMonsterPS, Location, FRotator::ZeroRotator);
			}
		}
	}
}


void APPWeaponInstantBase::Server_TakeDamageTo_Implementation(AActor* Victim, FVector Location)
{
	TakeDamageTo(Victim, Location);
}

void APPWeaponInstantBase::PlayPS(UParticleSystem* PS, FVector Location, FRotator Rotation)
{
	if (IsValid(PS))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PS, Location, Rotation);

		if (OwnerPawn->GetLocalRole() == ROLE_AutonomousProxy)
		{
			Server_PlayPS(PS, Location, Rotation);
		}
		else if (GetNetMode() == NM_ListenServer)
		{
			Multicast_PlayPS(PS, Location, Rotation);
		}
	}
}

void APPWeaponInstantBase::Server_PlayPS_Implementation(UParticleSystem* PS, FVector Location, FRotator Rotation)
{
	Multicast_PlayPS(PS, Location, Rotation);
}

void APPWeaponInstantBase::Multicast_PlayPS_Implementation(UParticleSystem* PS, FVector Location, FRotator Rotation)
{
	if (IsValid(OwnerPawn) && OwnerPawn->GetLocalRole() == ROLE_SimulatedProxy)
	{
		PlayPS(PS, Location, Rotation);
	}
}
