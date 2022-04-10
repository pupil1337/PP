// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPWeaponBase.h"
#include "PPWeaponInstantBase.generated.h"

USTRUCT()
struct FInstantInfo
{
	GENERATED_BODY()

	FVector StartLocation;
	FRotator StartRotation;

	FVector HitLocation;
	AActor* HitActor;
};

class UParticleSystem;

/**
 * 
 */
UCLASS()
class PPGAME_API APPWeaponInstantBase : public APPWeaponBase
{
	GENERATED_BODY()
	
public:
	APPWeaponInstantBase();
	virtual void BeginPlay() override;
	virtual bool Fire(bool Op) override;
	virtual void Aim(bool Op) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* TrailPS = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* HitPlayerPS = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* HitMonsterPS = nullptr;

	void TakeDamageTo(AActor* Victim, FVector Location);
	UFUNCTION(Server, Reliable)
	void Server_TakeDamageTo(AActor* Victim, FVector Location);

	void PlayPS(UParticleSystem* PS, FVector Location, FRotator Rotation);
	UFUNCTION(Server, Unreliable)
	void Server_PlayPS(UParticleSystem* PS, FVector Start, FRotator Rotation);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayPS(UParticleSystem* PS, FVector Start, FRotator Rotation);
};
