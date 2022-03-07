// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PPPlayerController.generated.h"

class APPCharacter;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PPGAME_API APPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnPossess(APawn* NewPawn) override;

	virtual void OnRep_Pawn() override;

private:
	void SetupCamera();

public:
	UPROPERTY(BlueprintReadOnly, Category = "ALS Player Controller")
	APPCharacter* PossessedCharacter = nullptr;

	UPROPERTY(Replicated, BlueprintReadOnly)
	FString PlayerName;
};
