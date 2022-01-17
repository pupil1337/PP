// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPCompBase.h"
#include "PPWeaponMgr.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PPGAME_API UPPWeaponMgr : public UPPCompBase
{
	GENERATED_BODY()
public:
	UPPWeaponMgr();

	virtual void PPInitComponent() override;
	virtual void ChangeControllerRole() override;
	
protected:
	UFUNCTION()
	void OnFire(bool Op);
};
