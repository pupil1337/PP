// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPWeaponBase.h"
#include "PPWeaponLauncherBase.generated.h"

class APPProjectileBase;

USTRUCT()
struct FSpawnProjectile
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APPProjectileBase> BulletClass;
	UPROPERTY(EditDefaultsOnly)
	float pro = 0.0f;
};

/**
 * 
 */
UCLASS()
class PPGAME_API APPWeaponLauncherBase : public APPWeaponBase
{
	GENERATED_BODY()

public:
	virtual bool Fire(bool Op) override;

	UFUNCTION(Server, Reliable)
	void Server_SpawnProjectile(FVector Location, FRotator Rotation);

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<FSpawnProjectile> BulletClass;
};
