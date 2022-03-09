// Copyright Epic Games, Inc. All Rights Reserved.


#include "PPGameGameModeBase.h"
#include "PPPlayerController.h"
#include "GameFramework/PlayerState.h"

void APPGameGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	APPPlayerController* tPlayerController = Cast<APPPlayerController>(NewPlayer);
	if (IsValid(tPlayerController))
	{
		AllPlayerControllers.Add(tPlayerController);
		APlayerState* tPlayerState = tPlayerController->GetPlayerState<APlayerState>();
		if (IsValid(tPlayerState))
		{
			tPlayerState->SetPlayerName(FString("P") + FString::FromInt(++Num));
		}
	}
}

void APPGameGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	AllPlayerControllers.Remove(Cast<APPPlayerController>(Exiting));
}
