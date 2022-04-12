// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPCompBase.h"
#include "PPInputBindComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFire, bool, Op);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAim, bool, Op);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChangeWeapon, bool, Up);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReload, bool, Start);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);

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
	FOnAim OnAim;
	FOnChangeWeapon OnChangeWeapon;
	FOnReload OnReload;
	FOnInteract OnInteract;

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
	void ReloadAction();
	UFUNCTION()
	void FirePressedAction();
	UFUNCTION()
	void FireReleasedAction();
	UFUNCTION()
	void WeaponUp();
	UFUNCTION()
	void WeaponDown();
	UFUNCTION()
	void InteractAction();

protected:
	UFUNCTION()
	void SwitchCameraMode();

	UFUNCTION()
	bool CheckOpenMirror(bool Up);
	
protected:
	bool bInputBind;
	FTimerHandle CameraMoveHandle;
	bool bFiring;
	
public:
	FORCEINLINE bool InFiring(){ return bFiring; };
	
public:
	
	virtual void PPInitComponent() override;
	
};
