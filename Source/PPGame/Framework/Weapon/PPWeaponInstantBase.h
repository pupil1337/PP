// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPWeaponBase.h"
#include "PPWeaponInstantBase.generated.h"

/**
 * 
 */
UCLASS()
class PPGAME_API APPWeaponInstantBase : public APPWeaponBase
{
	GENERATED_BODY()
	
public:
	APPWeaponInstantBase();

	virtual bool Fire() override;
};
