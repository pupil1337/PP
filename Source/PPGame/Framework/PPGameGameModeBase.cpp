// Copyright Epic Games, Inc. All Rights Reserved.


#include "PPGameGameModeBase.h"
#include "PPPlayerController.h"

void APPGameGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	APPPlayerController* tPlayerController = Cast<APPPlayerController>(NewPlayer);
	if (IsValid(tPlayerController))
	{
		AllPlayerControllers.Add(tPlayerController);
		tPlayerController->PlayerName = FString("P") + FString::FromInt(++Num);
	}
}

void APPGameGameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	AllPlayerControllers.Remove(Cast<APPPlayerController>(Exiting));
}
