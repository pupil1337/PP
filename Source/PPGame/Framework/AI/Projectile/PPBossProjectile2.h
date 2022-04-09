// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPGame/Framework/Projectile/PPProjectileBase.h"
#include "PPBossProjectile2.generated.h"

/**
 * 
 */
UCLASS()
class PPGAME_API APPBossProjectile2 : public APPProjectileBase
{
	GENERATED_BODY()
	
public:
	virtual void Destroyed() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APPProjectileBase> ProjectileClass;
};
