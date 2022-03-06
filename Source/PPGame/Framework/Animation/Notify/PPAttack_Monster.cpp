// Fill out your copyright notice in the Description page of Project Settings.


#include "PPAttack_Monster.h"

#include "Kismet/GameplayStatics.h"
#include "PPGame/Framework/PPCharacter.h"
#include "PPGame/Framework/AI/PPMonsterBase.h"
#include "PPGame/Framework/Component/PPAttributeComp.h"

void UPPAttack_Monster::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (IsValid(MeshComp))
	{
		APPMonsterBase* tMonster = Cast<APPMonsterBase>(MeshComp->GetOwner());
		if (IsValid(tMonster) && tMonster->GetLocalRole() == ENetRole::ROLE_Authority)
		{
			TArray<AActor*> FoundArray;
			UGameplayStatics::GetAllActorsOfClassWithTag(tMonster->GetWorld(), APPCharacter::StaticClass(), TEXT("Player"), FoundArray);
			if (FoundArray.Num() > 0)
			{
				for (auto& it: FoundArray)
				{
					APPCharacter* tPlayer = Cast<APPCharacter>(it);
					if (IsValid(tPlayer) && FVector::Distance(tMonster->GetActorLocation(), tPlayer->GetActorLocation()) < 200.0f)
					{
						UPPAttributeComp* tComp = Cast<UPPAttributeComp>(tPlayer->GetComponentByClass(UPPAttributeComp::StaticClass()));
						if (IsValid(tComp))
						{
							tComp->TakeDamage(tMonster, 10.0f);	
						}
					}
				}
			}
		}
	}
}
