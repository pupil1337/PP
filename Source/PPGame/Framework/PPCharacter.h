// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPCharacterBase.h"
#include "Library/PPCharacterEnumLibrary.h"
#include "PPCharacter.generated.h"

class UPPCameraAnimInstance;
class UPPPlayerAnimInstance;

/**
 * 
 */
UCLASS()
class PPGAME_API APPCharacter : public APPCharacterBase
{
	GENERATED_BODY()
	
public:

	APPCharacter();
	virtual void PreInitializeComponents() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
	UFUNCTION(BlueprintGetter, Category = "ALS|Character States")
	EPPRotationMode GetRotationMode() const { return RotationMode; }

	UFUNCTION(BlueprintGetter, Category = "ALS|Character States")
	EPPStance GetStance() const { return Stance; }

	UFUNCTION(BlueprintGetter, Category = "ALS|Character States")
	EPPViewMode GetViewMode() const { return ViewMode; }
	
	/** Camera System */

	UFUNCTION(BlueprintCallable, Category = "ALS|Camera System")
	virtual ECollisionChannel GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius);

	UFUNCTION(BlueprintCallable, Category = "ALS|Camera System")
	virtual FTransform GetThirdPersonPivotTarget();

	UFUNCTION(BlueprintCallable, Category = "ALS|Camera System")
	virtual FVector GetFirstPersonCameraTarget();

	UFUNCTION(BlueprintCallable, Category = "ALS|Camera System")
	void GetCameraParameters(float& TPFOVOut, float& FPFOVOut, bool& bRightShoulderOut) const;

	UFUNCTION(BlueprintCallable, Category = "ALS|Camera System")
	void SetCameraBehavior(UPPCameraAnimInstance* CamBeh) { CameraBehavior = CamBeh; }

	/** Move **/
	UFUNCTION(BlueprintGetter, Category = "ALS|Character States")
	EPPMovementState GetMovementState() const { return MovementState; }

	UFUNCTION(BlueprintGetter, Category = "ALS|Character States")
	EPPMovementAction GetMovementAction() const { return MovementAction; }

	void SetOverlayState(const EPPOverlayState NewOverlayState, bool bForce = false);
	void OnOverlayStateChanged(const EPPOverlayState PreOverlayState);
	
	void SetViewMode(const EPPViewMode NewViewMode, bool bForce = false);
	void OnViewModeChanged(const EPPViewMode PreViewMode);

	void SetRotationMode(const EPPRotationMode NewRotationMode, bool bForce = false);
	void OnRotationModeChanged(const EPPRotationMode PreRotationMode);

	void SetGait(const EPPGait NewGait, bool bForce = false);
	void OnGaitChanged(const EPPGait PreGait);

	UFUNCTION(Server, Reliable)
	void Server_SetOverlayState(const EPPOverlayState NewOverlayState, bool bForce = false);
	UFUNCTION()
	void OnRep_OverlayState(EPPOverlayState PreOverlayState);
	
	UFUNCTION(Server, Reliable)
	void Server_SetViewMode(const EPPViewMode NewViewMode, bool bForce = false);
	UFUNCTION()
	void OnRep_ViewMode(EPPViewMode PreViewMode);

	UFUNCTION(Server, Reliable)
	void Server_SetRotationMode(const EPPRotationMode NewRotationMode, bool bForce);
	UFUNCTION()
	void OnRep_RotationMode(EPPRotationMode PreRotationMode);

	UFUNCTION(Server, Reliable)
	void Server_SetGait(const EPPGait NewRotationMode, bool bForce);
	UFUNCTION()
	void OnRep_Gait(EPPGait PreGait);

	/** Other **/
	void UpdateCharacterMovement();
	void ForceUpdateAnimState();
	
protected:

	UPROPERTY(ReplicatedUsing = OnRep_OverlayState)
	EPPOverlayState OverlayState;
	UPROPERTY(ReplicatedUsing = OnRep_RotationMode)
	EPPRotationMode RotationMode;
	UPROPERTY(/*ReplicatedUsing = OnRep_Stance*/)
	EPPStance Stance;
	UPROPERTY(ReplicatedUsing = OnRep_ViewMode)
	EPPViewMode ViewMode;
	UPROPERTY(/*ReplicatedUsing = OnRep_Gait*/)
	EPPGait Gait;
	
	UPROPERTY()
	EPPMovementState MovementState;
	UPROPERTY()
	EPPMovementAction MovementAction;
	
	UPROPERTY()
	UPPPlayerAnimInstance* MainAnimInstance;
	UPROPERTY()
	UPPCameraAnimInstance* CameraBehavior;
	
	/** Camera System */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Camera System")
	float ThirdPersonFOV = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Camera System")
	float FirstPersonFOV = 90.0f;
	
};
