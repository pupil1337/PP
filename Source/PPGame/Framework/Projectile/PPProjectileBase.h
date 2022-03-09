// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPProjectileBase.generated.h"

class UCapsuleComponent;
class UProjectileMovementComponent;
class UParticleSystemComponent;

UCLASS()
class PPGAME_API APPProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:
	APPProjectileBase();

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnProjectileStop(const FHitResult& ImpactResult);
	
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CollisionComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* FlyParticleComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystem* ExplodParticleSystem;

	UPROPERTY()
	UParticleSystemComponent* ExplodParticleComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovementComponent = nullptr;

};
