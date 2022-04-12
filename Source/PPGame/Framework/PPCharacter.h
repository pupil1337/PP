// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPCharacterBase.h"
#include "Library/PPCharacterEnumLibrary.h"
#include "PPCharacter.generated.h"

class UPPCameraAnimInstance;
class UPPPlayerAnimInstance;
class UParticleSystem;
class UParticleSystemComponent;

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
	EPPOverlayState GetOverlayState() const { return OverlayState; }


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

	void SetCustomAction(const EPPCustomAction NewCustonAction, bool bForce = false);
	void OnCustActionChanged(const EPPCustomAction PreCustomAction);
	
	void SetMovementState(const EPPMovementState NewMovementState, bool bForce = false);
	void OnMovementStateChanged(const EPPMovementState NewMovementState);
	
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
	void Server_SetCustomAction(const EPPCustomAction NewCustonAction, bool bForce = false);
	UFUNCTION()
	void OnRep_CustomAction(EPPCustomAction PreCustomAction);

	UFUNCTION(Server, Reliable)
	void Server_SetMovementState(const EPPMovementState NewMovementState, bool bForce = false);
	UFUNCTION()
	void OnRep_MovementState(EPPMovementState PreMovementState);
	
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

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ShiftPS;
	UPROPERTY()
	UParticleSystemComponent* ShiftPSComp;
	UPROPERTY()
	FTimerHandle ShiftHandle;
	UFUNCTION()
	void OnShift();
	UFUNCTION(Server, Reliable)
	void Server_OnShift();
	void PlayShiftPS();
	UFUNCTION(NetMulticast, Unreliable)
	void Multi_PlayShiftPS();

	/** Other **/
	void UpdateCharacterMovement();
	FORCEINLINE float GetCameraAimPitch() { return AimPitch; }
	FORCEINLINE float GetCameraFirstFOV() { return FirstPersonFOV; }
	FORCEINLINE float GetCameraThirdFOV() { return ThirdPersonFOV; }
	UFUNCTION(Server, Unreliable)
	void Server_SetAimPitch(float NewAimPitch);
	
protected:

	UPROPERTY(ReplicatedUsing = OnRep_OverlayState)
	EPPOverlayState OverlayState = EPPOverlayState::Pistol;
	UPROPERTY(ReplicatedUsing = OnRep_CustomAction)
	EPPCustomAction CustomAction = EPPCustomAction::None;
	UPROPERTY(ReplicatedUsing = OnRep_RotationMode)
	EPPRotationMode RotationMode = EPPRotationMode::LookingDirection;
	UPROPERTY(ReplicatedUsing = OnRep_MovementState)
	EPPMovementState MovementState = EPPMovementState::Grounded;
	UPROPERTY()
	EPPMovementAction MovementAction = EPPMovementAction::None;
	UPROPERTY(/*ReplicatedUsing = OnRep_Stance*/)
	EPPStance Stance = EPPStance::Standing;
	UPROPERTY(ReplicatedUsing = OnRep_ViewMode)
	EPPViewMode ViewMode = EPPViewMode::ThirdPerson;
	UPROPERTY(/*ReplicatedUsing = OnRep_Gait*/)
	EPPGait Gait = EPPGait::Idle;
	UPROPERTY(Replicated)
	float AimPitch;
	
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
