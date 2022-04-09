// Fill out your copyright notice in the Description page of Project Settings.


#include "PPPlayerProjectile1.h"
#include "Kismet/GameplayStatics.h"
#include "PPGame/Framework/AI/PPMonsterBase.h"
#include "PPGame/Framework/PPCharacter.h"
#include "PPGame/Framework/Component/PPAttributeComp.h"

void APPPlayerProjectile1::OnProjectileStop(const FHitResult& ImpactResult)
{
	Super::OnProjectileStop(ImpactResult);

	
}
