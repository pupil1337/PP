// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPCompBase.h"
#include "PPInputBindComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFire, bool, Op);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeWeapon, bool, Up);

class UInputComponent;

/**
 * 
 */
UCLASS()
class PPGAME_API UPPInputBindComp : public UPPCompBase
{
	GENERATED_BODY()
public:
	UPPInputBindComp();

	FOnFire OnFire;
	FOnChangeWeapon OnChangeWeapon;

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
	UFUNCTION()
	void JumpAction();
	UFUNCTION()
	void FirePressedAction();
	UFUNCTION()
	void FireReleasedAction();
	UFUNCTION()
	void WeaponUp();
	UFUNCTION()
	void WeaponDown();

protected:
	UFUNCTION()
	void SwitchCameraMode();
	
protected:
	bool bInputBind;
	FTimerHandle CameraMoveHandle;
	bool bFiring;
	
public:
	FORCEINLINE bool InFiring(){ return bFiring; };
	
public:
	
	virtual void PPInitComponent() override;
	
};
