// Fill out your copyright notice in the Description page of Project Settings.


#include "PPNotify_Dead.h"

#include "PPGame/Framework/PPCharacter.h"

void UPPNotify_Dead::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (IsValid(MeshComp))
	{
		APPCharacter* tPlayer = Cast<APPCharacter>(MeshComp->GetOwner());
		if (IsValid(tPlayer) && tPlayer->GetLocalRole() == ROLE_Authority)
		{
			tPlayer->SetActorHiddenInGame(true);
		}
	}
}
