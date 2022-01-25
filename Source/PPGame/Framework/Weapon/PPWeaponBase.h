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
	FRotator MuzzleRotation;
};

USTRUCT()
struct FTraceResult
{
	GENERATED_BODY()

	FVector FireLocation;
	FRotator FireRotation;

	FVector HitLocation;
	UPROPERTY()
	AActor* HitActor = nullptr;
	
};

USTRUCT()
struct FWeaponCfg
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float Damage = 10.0f;

	UPROPERTY(EditDefaultsOnly)
	int ClipSize = 30;
	UPROPERTY(EditDefaultsOnly)
	int SpareSize = 300;
	
	UPROPERTY(EditDefaultsOnly)
	float ValidDistance = 100000.0f;
};

class APPCharacter;
class UPPCrosshairWidget;
class UParticleSystem;
class UParticleSystemComponent;
class USoundCue;

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
	virtual bool Fire(bool Op);
	virtual void Aim(bool Op);
	virtual bool Reload(bool Start);
	
	UPROPERTY(EditDefaultsOnly)
	EPPOverlayState WeaponType;

protected:
	FVector GetMuzzleLocation();
	void CalcTraceResult(FTraceResult& Result);
	virtual void TakeDamageTo(AActor* Victim);
	UFUNCTION(Server, Reliable)
	virtual void Server_TakeDamageTo(AActor* Victim);
	void PlayEffect();
	UFUNCTION(Server, Unreliable)
	void Server_PlayEffect();
	UFUNCTION(NetMulticast, Unreliable)
	void Multi_PlayEffect();
	UFUNCTION()
	void OnWeaponEvent();
	
protected:
	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	FWeaponCfg WeaponCfg;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UPPCrosshairWidget> CrosshairWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* MuzzlePS;

	UPROPERTY(EditDefaultsOnly, Category="PP|Sounds")
	USoundCue* FireSoundCue;

	UPROPERTY()
	UParticleSystemComponent* PSComp;
	
	UPROPERTY()
	UPPCrosshairWidget* Crosshair;

	UPROPERTY()
	APPCharacter* OwnerPawn;

	/** Transient **/
	UPROPERTY()
	FCurrFireInfo CurrFireInfo;

public:
	UPROPERTY(BlueprintReadOnly)
	int CurrClipSize;
	UPROPERTY(BlueprintReadOnly)
	int SpareClipSize;
};
