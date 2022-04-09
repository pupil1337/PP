// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PPBossFireTrigger2.generated.h"

class APPProjectileBase;

/**
 * 
 */
UCLASS()
class PPGAME_API UPPBossFireTrigger2 : public UAnimNotify
{
	GENERATED_BODY()
	
private:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<APPProjectileBase> ProjectileClass;

	UPROPERTY(EditAnywhere)
	FName MuzzleName;
};
