// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCharacter.h"

#include "Animation/PPCameraAnimInstance.h"
#include "Animation/PPPlayerAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "PPGame/Framework/Component/PPInputBindComp.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

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
	DOREPLIFETIME_CONDITION(APPCharacter, CustomAction, COND_SkipOwner);
}

void APPCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocallyControlled())
	{
		UPPInputBindComp* tInputBindComp = Cast<UPPInputBindComp>(GetComponentByClass(UPPInputBindComp::StaticClass()));
		if (IsValid(tInputBindComp))
		{
			tInputBindComp->OnShift.AddUniqueDynamic(this, &APPCharacter::OnShift);
		}
	}
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
		APlayerController* tPC = Cast<APlayerController>(GetController());
		if (IsValid(tPC) && IsValid(tPC->PlayerCameraManager))
		{
			AimPitch = tPC->PlayerCameraManager->ViewTarget.POV.Rotation.Pitch;	
			if (GetNetMode() == NM_Client)
			{
				Server_SetAimPitch(AimPitch);
			}
		}
	}

	if (GetLocalRole() == ROLE_Authority)
	{

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

	if (CameraBehavior)
	{
		CameraBehavior->OverlayState = OverlayState;
	}
}

void APPCharacter::SetCustomAction(const EPPCustomAction NewCustonAction, bool bForce)
{
	if (CustomAction != NewCustonAction || bForce)
	{
		const EPPCustomAction Prev = CustomAction;
		CustomAction = NewCustonAction;
		OnCustActionChanged(Prev);

		if (GetLocalRole() == ROLE_AutonomousProxy)
		{
			Server_SetCustomAction(NewCustonAction, bForce);
		}
	}
}

void APPCharacter::OnCustActionChanged(const EPPCustomAction PreCustomAction)
{
	if (IsValid(MainAnimInstance))
	{
		MainAnimInstance->GetAnimInfo().CustomAction = CustomAction;
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

	if (RotationMode == EPPRotationMode::Aiming)
	{
		bUseControllerRotationYaw = true;
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

void APPCharacter::OnShift()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		float Value = 8000.0f;
		if (GetCharacterMovement()->MovementMode == EMovementMode::MOVE_Falling)
		{
			Value = 1000.0f;
		}
		GetCharacterMovement()->AddImpulse(GetActorForwardVector() * Value, true);
		Multi_PlayShiftPS();
	}
	else if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		Server_OnShift();
		PlayShiftPS();
	}
}

void APPCharacter::Server_OnShift_Implementation()
{
	OnShift();
}

void APPCharacter::PlayShiftPS()
{
	if (IsValid(ShiftPS))
	{
		FTimerDelegate Func = FTimerDelegate::CreateLambda([this]() {
			if (IsValid(ShiftPSComp))
			{
				if (ShiftPSComp->IsActive())
				{
					ShiftPSComp->Deactivate();
				}
				ShiftPSComp->DestroyComponent(true);
			}
		});

		Func.ExecuteIfBound();
		ShiftPSComp = UGameplayStatics::SpawnEmitterAttached(ShiftPS, GetMesh(), NAME_None, { 0.0f, 20.0f, 50.0f }, { 0.0f, -90.0f, 0.0f });
		GetWorldTimerManager().ClearTimer(ShiftHandle);
		GetWorldTimerManager().SetTimer(ShiftHandle, Func, 0.2f, false);
	}
}

void APPCharacter::Multi_PlayShiftPS_Implementation()
{
	if (!IsNetMode(NM_DedicatedServer) && GetLocalRole() != ROLE_AutonomousProxy)
	{
		PlayShiftPS();
	}
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

void APPCharacter::OnRep_CustomAction(EPPCustomAction PreCustomAction)
{
	OnCustActionChanged(PreCustomAction);
}

void APPCharacter::Server_SetCustomAction_Implementation(const EPPCustomAction NewCustonAction, bool bForce)
{
	SetCustomAction(NewCustonAction, bForce);
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
	ENetMode tMode = GetNetMode();
	ENetRole tRole = GetLocalRole();
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
