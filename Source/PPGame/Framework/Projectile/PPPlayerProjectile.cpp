// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPlayerProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "PPGame/Framework/AI/PPMonsterBase.h"
#include "PPGame/Framework/PPCharacter.h"
#include "PPGame/Framework/Component/PPAttributeComp.h"

void APPPlayerProjectile::OnProjectileStop(const FHitResult& ImpactResult)
{
	Super::OnProjectileStop(ImpactResult);

	if (GetLocalRole() == ROLE_Authority)
	{
		// ´òµ½¹ÖÎï
		if (ExplodMonsterArray.Num() > 0)
		{
			for (auto& it : ExplodMonsterArray)
			{
				APPMonsterBase* tMonster = Cast<APPMonsterBase>(it);
				if (IsValid(tMonster))
				{
					tMonster->SetDamageDeBuff(DamageType, GetOwner());
				}
			}
		}
	}
}
