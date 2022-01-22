// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "PPNotify_Reload.generated.h"

class APPCharacter;
class UPPWeaponMgr;

/**
 * 
 */
UCLASS()
class PPGAME_API UPPNotify_Reload : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
