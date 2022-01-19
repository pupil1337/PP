// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPWeaponBase.h"
#include "PPWeaponInstantBase.generated.h"

class UPPCrosshairWidget;

/**
 * 
 */
UCLASS()
class PPGAME_API APPWeaponInstantBase : public APPWeaponBase
{
	GENERATED_BODY()
	
public:
	APPWeaponInstantBase();
	virtual void BeginPlay() override;
	virtual bool Fire(bool Op) override;
	virtual void Aim(bool Op) override;
	
};
