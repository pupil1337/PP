// Fill out your copyright notice in the Description page of Project Settings.


#include "PPBossFireTrigger2.h"
#include "PPGame/Framework/AI/PPBossBase.h"
#include "PPGame/Framework/AI/Projectile/PPBossProjectile1.h"


void UPPBossFireTrigger2::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (IsValid(MeshComp))
	{
		APPBossBase* tBoss = Cast<APPBossBase>(MeshComp->GetOwner());
		if (IsValid(tBoss) && tBoss->GetLocalRole() == ROLE_Authority)
		{
			if (!MuzzleName.IsNone())
			{
				UClass* tClass = ProjectileClass.Get();
				if (IsValid(tClass))
				{
					FVector tBossV = tBoss->GetActorLocation();
					FVector tMuzzleV = MeshComp->GetSocketLocation(MuzzleName);
					FVector tV = tMuzzleV - tBossV;
					FRotator tR = tV.Rotation();
					tR.Normalize();

					FActorSpawnParameters tParams;
					tParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					tParams.Owner = tBoss;
					tBoss->GetWorld()->SpawnActor<APPProjectileBase>(tClass, tMuzzleV, tR, tParams);
				}
			}
		}
	}
}
