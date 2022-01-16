// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PPGame/Framework/Library/PPCharacterEnumLibrary.h"
#include "PPPlayerAnimInstance.generated.h"

USTRUCT(BlueprintType)
struct FPPAnimInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly)
	EPPOverlayState OverlayState = EPPOverlayState::Pistol;

	UPROPERTY(BlueprintReadOnly)
	EPPRotationMode RotationMode = EPPRotationMode::LookingDirection;

	UPROPERTY(BlueprintReadOnly)
	EPPGait Gait = EPPGait::Idle;
	
	UPROPERTY(BlueprintReadOnly)
	EPPViewMode ViewMode = EPPViewMode::ThirdPerson;

	UPROPERTY(BlueprintReadOnly)
	float MoveDir;
};

class APPCharacter;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PPGAME_API UPPPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	FORCEINLINE FPPAnimInfo& GetAnimInfo() { return PPAnimInfo; }
	
protected:
	UPROPERTY(BlueprintReadOnly)
	APPCharacter* OwnerPawn;
	
	UPROPERTY(BlueprintReadOnly)
	FPPAnimInfo PPAnimInfo;

private:
	void UpdateMoveDir();
	FVector LastPos;
};
