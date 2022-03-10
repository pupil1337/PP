
// Fill out your copyright notice in the Description page of Project Settings.


#include "PPNotify_Fire.h"

#include "PPGame/Framework/PPCharacter.h"
#include "PPGame/Framework/Component/PPWeaponMgr.h"

void UPPNotify_Fire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (IsValid(MeshComp))
	{
		APPCharacter* tPlayer = Cast<APPCharacter>(MeshComp->GetOwner());
		if (IsValid(tPlayer))
		{
			UPPWeaponMgr* tComp = Cast<UPPWeaponMgr>(tPlayer->GetComponentByClass(UPPWeaponMgr::StaticClass()));
			if (IsValid(tComp))
			{
				if (tPlayer->IsLocallyControlled())
				{
                  	tComp->OnFire(false);
				}
			}
		}
	}
}
