// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PPGameGameModeBase.generated.h"

class APPPlayerController;

/**
 * 
 */
UCLASS()
class PPGAME_API APPGameGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<APPPlayerController*> AllPlayerControllers;

	int Num;
};
