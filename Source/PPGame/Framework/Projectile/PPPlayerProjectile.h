// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPGame/Framework/Projectile/PPProjectileBase.h"
#include "PPPlayerProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PPGAME_API APPPlayerProjectile : public APPProjectileBase
{
	GENERATED_BODY()
	
protected:
	virtual void OnProjectileStop(const FHitResult& ImpactResult) override;

};
