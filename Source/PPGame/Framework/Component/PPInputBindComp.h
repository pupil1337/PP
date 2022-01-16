// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPCompBase.h"
#include "PPInputBindComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoveForward, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMoveRight, float, Value);

class UInputComponent;
class APPCharacter;
/**
 * 
 */
UCLASS()
class PPGAME_API UPPInputBindComp : public UPPCompBase
{
	GENERATED_BODY()
public:
	UPPInputBindComp();

	FOnMoveForward OnMoveForward;
	FOnMoveRight OnMoveRight;

	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);
	UFUNCTION()
	void MoveForward(float Value);
	UFUNCTION()
	void MoveRight(float Value);
	UFUNCTION()
	void TurnUp(float Value);
	UFUNCTION()
	void TurnRight(float Value);
	UFUNCTION()
	void CameraReleasedAction();
	UFUNCTION()
	void AimPressedAction();
	UFUNCTION()
	void AimReleasedAction();

protected:
	UFUNCTION()
	void SwitchCameraMode();
	
protected:
	bool bInputBind;
	FTimerHandle CameraMoveHandle;
	
	UPROPERTY()
	APPCharacter* OwnerPawn;
	
public:
	
	virtual void PPInitComponent() override;
	
};
