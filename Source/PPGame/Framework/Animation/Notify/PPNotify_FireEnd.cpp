// Fill out your copyright notice in the Description page of Project Settings.


#include "PPNotify_FireEnd.h"

#include "PPGame/Framework/PPCharacter.h"
#include "PPGame/Framework/Component/PPWeaponMgr.h"


void UPPNotify_FireEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	if (IsValid(MeshComp))
	{
		APPCharacter* tPlayer = Cast<APPCharacter>(MeshComp->GetOwner());
		if (IsValid(tPlayer) && tPlayer->IsLocallyControlled())
		{
			UPPWeaponMgr* tComp = Cast<UPPWeaponMgr>(tPlayer->GetComponentByClass(UPPWeaponMgr::StaticClass()));
			if (IsValid(tComp))
			{
				tComp->OnFireState(false);
				tComp->OnFire();
			}
		}
	}
}
