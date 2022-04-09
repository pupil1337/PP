// Fill out your copyright notice in the Description page of Project Settings.


#include "PPBossFireTrigger1.h"
#include "PPGame/Framework/AI/PPBossBase.h"
#include "PPGame/Framework/PPCharacter.h"
#include "PPGame/Framework/Projectile/PPProjectileBase.h"

void UPPBossFireTrigger1::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (IsValid(MeshComp))
	{
		APPBossBase* tBoss = Cast<APPBossBase>(MeshComp->GetOwner());
		if (IsValid(tBoss))
		{
			if (tBoss->GetLocalRole() == ROLE_Authority)
			{
				if (!MuzzleName.IsNone() && IsValid(ProjectileClass))
				{
					UClass* tClass = ProjectileClass.Get();
					if (IsValid(tClass))
					{
						APPCharacter* tPlayer = tBoss->GetEnemy();
						if (IsValid(tPlayer))
						{
							FVector tStartV = MeshComp->GetSocketLocation(MuzzleName);
							FVector tEndV = tPlayer->GetActorLocation();
							FVector tV = tEndV - tStartV;
							FRotator tR = tV.Rotation();
							tR.Normalize();

							FActorSpawnParameters tParams;
							tParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
							tParams.Owner = tBoss;
							tBoss->GetWorld()->SpawnActor<APPProjectileBase>(tClass, tStartV, tR, tParams);
						}
					}
				}
			}
		}
	}
}
