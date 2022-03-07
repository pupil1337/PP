// Fill out your copyright notice in the Description page of Project Settings.

#include "PPPlayerController.h"

#include "PPCharacter.h"
#include "PPPlayerCameraManager.h"
#include "Net/UnrealNetwork.h"

void APPPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APPPlayerController, PlayerName);
}

void APPPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);
	PossessedCharacter = Cast<APPCharacter>(NewPawn);
	if (!IsRunningDedicatedServer())
	{
		// Servers want to setup camera only in listen servers.
		SetupCamera();
	}
}

void APPPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	PossessedCharacter = Cast<APPCharacter>(GetPawn());
	SetupCamera();
}

void APPPlayerController::SetupCamera()
{
	// Call "OnPossess" in Player Camera Manager when possessing a pawn
	APPPlayerCameraManager* CastedMgr = Cast<APPPlayerCameraManager>(PlayerCameraManager);
	if (PossessedCharacter && CastedMgr)
	{
		CastedMgr->OnPossess(PossessedCharacter);
	}
}
