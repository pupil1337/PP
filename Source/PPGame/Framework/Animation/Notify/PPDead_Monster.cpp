// Fill out your copyright notice in the Description page of Project Settings.


#include "PPDead_Monster.h"

#include "Kismet/GameplayStatics.h"
#include "PPGame/Framework/AI/PPMonsterBase.h"

void UPPDead_Monster::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (IsValid(MeshComp))
	{
		APPMonsterBase* tMonster = Cast<APPMonsterBase>(MeshComp->GetOwner());
		if (IsValid(tMonster) && tMonster->GetLocalRole() == ROLE_Authority)
		{
			tMonster->GetWorld()->DestroyActor(tMonster, true);
		}
	}
}
