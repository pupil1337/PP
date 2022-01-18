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

class APPCharacter;

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
	
	UPROPERTY(EditDefaultsOnly)
	EPPOverlayState WeaponType;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* Mesh;

	UPROPERTY()
	APPCharacter* OwnerPawn;

	/** Transient **/
	UPROPERTY(Transient)
	FCurrFireInfo CurrFireInfo;
};
