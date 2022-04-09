// Fill out your copyright notice in the Description page of Project Settings.


#include "PPBossProjectile2.h"
#include "PPGame/Framework/AI/PPBossBase.h"
#include "Kismet/GameplayStatics.h"
#include "PPGame/Framework/PPCharacter.h"
#include "Kismet/KismetMathLibrary.h"


void APPBossProjectile2::Destroyed()
{
	Super::Destroyed();

	APPBossBase* tBoss = Cast<APPBossBase>(GetOwner());
	if (IsValid(tBoss) && tBoss->GetLocalRole() == ROLE_Authority)
	{
		UClass* tClass = ProjectileClass.Get();
		if (IsValid(tClass))
		{
			TArray<AActor*> tFoundPlayerArray;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APPCharacter::StaticClass(), tFoundPlayerArray);
			if (tFoundPlayerArray.Num() > 0)
			{
				APPCharacter* tPlayer = Cast<APPCharacter>(tFoundPlayerArray[UKismetMathLibrary::RandomInteger(tFoundPlayerArray.Num())]);
				if (IsValid(tPlayer))
				{
					FVector tV = tPlayer->GetActorLocation() + FVector::UpVector * 2000.0f;
					tV += FVector::UpVector * 1000.0f * UKismetMathLibrary::RandomFloat();

					FActorSpawnParameters tParams;
					tParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
					tParams.Owner = tBoss;
					GetWorld()->SpawnActor<APPProjectileBase>(tClass, tV + FVector::UpVector * 10000.0f, { -90.0f, 0.0f, 0.0f }, tParams);
				}
			}
		}
	}
}
