// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PPPlayerCameraManager.generated.h"

class APPCharacter;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PPGAME_API APPPlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	APPPlayerCameraManager();

	UFUNCTION(BlueprintCallable, Category = "ALS|Camera")
	void OnPossess(APPCharacter* NewCharacter);

	UFUNCTION(BlueprintCallable, Category = "ALS|Camera")
	float GetCameraBehaviorParam(FName CurveName) const;

protected:
	virtual void UpdateViewTargetInternal(FTViewTarget& OutVT, float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "ALS|Camera")
	static FVector CalculateAxisIndependentLag(
		FVector CurrentLocation, FVector TargetLocation, FRotator CameraRotation, FVector LagSpeeds, float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = "ALS|Camera")
	bool CustomCameraBehavior(float DeltaTime, FVector& Location, FRotator& Rotation, float& FOV, bool bSniperMirror);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ALS|Camera")
	APPCharacter* ControlledCharacter = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ALS|Camera")
	USkeletalMeshComponent* CameraBehavior = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ALS|Camera")
	FVector RootLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ALS|Camera")
	FTransform SmoothedPivotTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ALS|Camera")
	FVector PivotLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ALS|Camera")
	FVector TargetCameraLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ALS|Camera")
	FRotator TargetCameraRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Camera")
	FRotator DebugViewRotation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|Camera")
	FVector DebugViewOffset;
};
