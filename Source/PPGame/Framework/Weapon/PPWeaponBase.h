// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPGame/Framework/Library/PPCharacterEnumLibrary.h"
#include "PPWeaponBase.generated.h"

USTRUCT()
struct FCurrFireInfo
{
	GENERATED_BODY()

	FVector CameraLocation;
	FRotator CameraRotation;
	FVector MuzzleLocation;
};

USTRUCT()
struct FTraceResult
{
	GENERATED_BODY()

	FVector FireLocation;
	FRotator FireRotation;

	FVector HitLocation;
	AActor* HitActor;
	
};

USTRUCT()
struct FWeaponCfg
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float ValidDistance = 10000.0f;
};

class APPCharacter;
class UPPCrosshairWidget;
class UParticleSystem;
class UParticleSystemComponent;

UCLASS()
class PPGAME_API APPWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	APPWeaponBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Equip();
	virtual void UnEquip();
	virtual bool Fire();
	virtual void Aim(bool Op);

	UFUNCTION()
	void PlayMuzzlePS();
	
	UPROPERTY(EditDefaultsOnly)
	EPPOverlayState WeaponType;

protected:
	FVector GetMuzzleLocation();
	void CalcTraceResult(FTraceResult& Result);
	
protected:
	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	FWeaponCfg WeaponCfg;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPPCrosshairWidget> CrosshairWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* MuzzlePS;

	UPROPERTY()
	UParticleSystemComponent* PSComp;
	
	UPROPERTY()
	UPPCrosshairWidget* Crosshair;

	UPROPERTY()
	APPCharacter* OwnerPawn;

	/** Transient **/
	FCurrFireInfo CurrFireInfo;
};
