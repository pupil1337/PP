// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPPanelWidget.generated.h"

class APPWeaponBase;

/**
 * 
 */
UCLASS()
class PPGAME_API UPPPanelWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealth(float NewHealth);
	UFUNCTION(BlueprintImplementableEvent)
	void OnWeapon(int CurrAmmo, int SpareAmmo);
};
