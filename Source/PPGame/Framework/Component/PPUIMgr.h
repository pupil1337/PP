// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPCompBase.h"
#include "PPUIMgr.generated.h"

class UPPPanelWidget;
class APPWeaponBase;

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PPGAME_API UPPUIMgr : public UPPCompBase
{
	GENERATED_BODY()

public:
	UPPUIMgr();

	virtual void PPInitComponent() override;

	void OnHealth(float NewHealth);
	void OnWeapon(int CurrAmmo, int SpareAmmo);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPPPanelWidget> PanelClass;
	UPROPERTY()
	UPPPanelWidget* Panel = nullptr;
	
};
