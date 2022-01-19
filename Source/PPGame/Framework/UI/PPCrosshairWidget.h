// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPCrosshairWidget.generated.h"

/**
 * 
 */
UCLASS()
class PPGAME_API UPPCrosshairWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Aim(bool Op);
	UFUNCTION(BlueprintImplementableEvent)
	void Hit();
};
