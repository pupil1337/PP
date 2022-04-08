// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPlayerProjectile1.h"
#include "Kismet/GameplayStatics.h"
#include "PPGame/Framework/AI/PPMonsterBase.h"
#include "PPGame/Framework/PPCharacter.h"
#include "PPGame/Framework/Component/PPAttributeComp.h"

void APPPlayerProjectile1::OnProjectileStop(const FHitResult& ImpactResult)
{
	Super::OnProjectileStop(ImpactResult);

	if (GetLocalRole() == ROLE_Authority)
	{
		// 打到怪
		TArray<AActor*> tFoundMonsterArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APPMonsterBase::StaticClass(), tFoundMonsterArray);
		for (auto& it : tFoundMonsterArray)
		{
			APPMonsterBase* tMonster = Cast<APPMonsterBase>(it);
			if (IsValid(tMonster))
			{
				if (FVector::Distance(tMonster->GetActorLocation(), GetActorLocation()) < ExplodRadius)
				{
					tMonster->MonsterTakeDamage(ExplodDamage, GetOwner());
				}
			}
		}

		// 打到玩家
		TArray<AActor*> tFoundPlayerArray;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APPCharacter::StaticClass(), tFoundPlayerArray);
		for (auto& it : tFoundPlayerArray)
		{
			APPCharacter* tPlayer = Cast<APPCharacter>(it);
			if (IsValid(tPlayer))
			{
				if (FVector::Distance(tPlayer->GetActorLocation(), GetActorLocation()) < ExplodRadius)
				{
					UPPAttributeComp* tComp = Cast<UPPAttributeComp>(tPlayer->GetComponentByClass(UPPAttributeComp::StaticClass()));
					if (IsValid(tComp))
					{
						tComp->TakeDamage(GetOwner(), 10.0f);
					}
				}
			}
		}
	}
}
