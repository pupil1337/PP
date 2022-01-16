// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCharacter.h"

#include "Animation/PPCameraAnimInstance.h"
#include "Animation/PPPlayerAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Net/UnrealNetwork.h"

const FName NAME_FP_Camera(TEXT("FP_Camera"));

APPCharacter::APPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APPCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	if (GetMesh())
	{
		MainAnimInstance = Cast<UPPPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	}
}

void APPCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(APPCharacter, ViewMode, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(APPCharacter, RotationMode, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(APPCharacter, OverlayState, COND_SkipOwner);
	DOREPLIFETIME_CONDITION(APPCharacter, AimPitch, COND_SkipOwner);
}

void APPCharacter::BeginPlay()
{
	Super::BeginPlay();

	ForceUpdateAnimState();
}

void APPCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (MovementState == EPPMovementState::Grounded)
	{
		UpdateCharacterMovement();
	}
	
	if (IsLocallyControlled())
	{
		AimPitch = Cast<APlayerController>(GetController())->PlayerCameraManager->ViewTarget.POV.Rotation.Pitch;
		if (GetNetMode() == NM_Client)
		{
			Server_SetAimPitch(AimPitch);
		}
	}
}

ECollisionChannel APPCharacter::GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius)
{
	TraceOrigin = GetActorLocation();
	TraceRadius = 10.0f;
	return ECC_Visibility;
}

FTransform APPCharacter::GetThirdPersonPivotTarget()
{
	return GetActorTransform();
}

FVector APPCharacter::GetFirstPersonCameraTarget()
{
	return GetMesh()->GetSocketLocation(NAME_FP_Camera);
}

void APPCharacter::GetCameraParameters(float& TPFOVOut, float& FPFOVOut, bool& bRightShoulderOut) const
{
	TPFOVOut = ThirdPersonFOV;
	FPFOVOut = FirstPersonFOV;
}

void APPCharacter::SetOverlayState(const EPPOverlayState NewOverlayState, bool bForce)
{
	if (OverlayState != NewOverlayState || bForce)
	{
		const EPPOverlayState Prev = OverlayState;
		OverlayState = NewOverlayState;
		OnOverlayStateChanged(Prev);

		if (GetLocalRole() == ROLE_AutonomousProxy)
		{
			Server_SetOverlayState(NewOverlayState, bForce);
		}
	}
}

void APPCharacter::OnOverlayStateChanged(const EPPOverlayState PreOverlayState)
{
	if (IsValid(MainAnimInstance))
	{
		MainAnimInstance->GetAnimInfo().OverlayState = OverlayState;
	}
}

void APPCharacter::SetMovementState(const EPPMovementState NewMovementState, bool bForce)
{
	if (bForce || MovementState != NewMovementState)
	{
		const EPPMovementState Prev = MovementState;
		MovementState = NewMovementState;
		OnMovementStateChanged(Prev);
	}
}

void APPCharacter::OnMovementStateChanged(const EPPMovementState NewMovementState)
{
	if (MainAnimInstance)
	{
		MainAnimInstance->GetAnimInfo().MovementState = MovementState;
	}

	if (CameraBehavior)
	{
		CameraBehavior->MovementState = MovementState;
	}
}

void APPCharacter::SetViewMode(const EPPViewMode NewViewMode, bool bForce)
{
	if (ViewMode != NewViewMode || bForce)
	{
		const EPPViewMode Prev = ViewMode;
		ViewMode = NewViewMode;
		OnViewModeChanged(Prev);

		if (GetLocalRole() == ROLE_AutonomousProxy)
		{
			Server_SetViewMode(NewViewMode, bForce);
		}
	}
}

void APPCharacter::OnViewModeChanged(const EPPViewMode PreViewMode)
{
	if (IsValid(MainAnimInstance))
	{
		MainAnimInstance->GetAnimInfo().ViewMode = ViewMode;
	}

	if (IsValid(CameraBehavior))
	{
		CameraBehavior->ViewMode = ViewMode;
	}
}

void APPCharacter::SetRotationMode(const EPPRotationMode NewRotationMode, bool bForce)
{
	if (bForce || RotationMode != NewRotationMode)
	{
		const EPPRotationMode Prev = RotationMode;
		RotationMode = NewRotationMode;
		OnRotationModeChanged(Prev);

		if (GetLocalRole() == ROLE_AutonomousProxy)
		{
			Server_SetRotationMode(NewRotationMode, bForce);
		}
	}
}

void APPCharacter::OnRotationModeChanged(const EPPRotationMode PreRotationMode)
{
	if (IsValid(MainAnimInstance))
	{
		MainAnimInstance->GetAnimInfo().RotationMode = RotationMode;
	}

	if (CameraBehavior)
	{
		CameraBehavior->SetRotationMode(RotationMode);
	}
}

void APPCharacter::SetGait(const EPPGait NewGait, bool bForce)
{
	if (Gait != NewGait || bForce)
	{
		const EPPGait Prev = Gait;
		Gait = NewGait;
		OnGaitChanged(Prev);
	}
}

void APPCharacter::OnGaitChanged(const EPPGait PreGait)
{
	if (IsValid(MainAnimInstance))
	{
		MainAnimInstance->GetAnimInfo().Gait = Gait;
	}

	if (IsValid(CameraBehavior))
	{
		CameraBehavior->Gait = Gait;
	}
}

void APPCharacter::UpdateCharacterMovement()
{
	if (GetNetMode() != NM_DedicatedServer)
	{
		if (GetMovementComponent()->Velocity.Size() > 5.0f)
		{
			if (Gait == EPPGait::Idle)
			{
				SetGait(EPPGait::Running);
			}
		}
		else
		{
			if (Gait == EPPGait::Running)
			{
				SetGait(EPPGait::Idle);
			}
		}
	}
}

void APPCharacter::ForceUpdateAnimState()
{
	SetViewMode(EPPViewMode::ThirdPerson, true);
	SetRotationMode(EPPRotationMode::LookingDirection, true);
	SetGait(EPPGait::Idle, true);
	SetOverlayState(EPPOverlayState::Pistol, true);
	SetMovementState(EPPMovementState::Grounded, true);
}

void APPCharacter::Server_SetAimPitch_Implementation(float NewAimPitch)
{
	AimPitch = NewAimPitch;
}


void APPCharacter::OnRep_MovementState(EPPMovementState PreMovementState)
{
	OnMovementStateChanged(PreMovementState);
}

void APPCharacter::Server_SetMovementState_Implementation(const EPPMovementState NewMovementState, bool bForce)
{
	SetMovementState(NewMovementState, bForce);
}

void APPCharacter::OnRep_OverlayState(EPPOverlayState PreOverlayState)
{
	OnOverlayStateChanged(PreOverlayState);
}

void APPCharacter::Server_SetOverlayState_Implementation(const EPPOverlayState NewOverlayState, bool bForce)
{
	SetOverlayState(NewOverlayState, bForce);
}

void APPCharacter::OnRep_ViewMode(EPPViewMode PreViewMode)
{
	OnViewModeChanged(PreViewMode);
}

void APPCharacter::Server_SetViewMode_Implementation(const EPPViewMode NewViewMode, bool bForce)
{
	SetViewMode(NewViewMode, bForce);
}

void APPCharacter::OnRep_RotationMode(EPPRotationMode PreRotationMode)
{
	OnRotationModeChanged(PreRotationMode);
}

void APPCharacter::Server_SetRotationMode_Implementation(const EPPRotationMode NewRotationMode, bool bForce)
{
	SetRotationMode(NewRotationMode, bForce);
}

void APPCharacter::OnRep_Gait(EPPGait PreGait)
{
	OnGaitChanged(PreGait);	
}

void APPCharacter::Server_SetGait_Implementation(const EPPGait PreGait, bool bForce)
{
	SetGait(PreGait, bForce);
}
