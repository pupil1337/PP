// Fill out your copyright notice in the Description page of Project Settings.


#include "PPWeaponMgr.h"

#include "PPInputBindComp.h"
#include "PPGame/Framework/PPCharacter.h"


UPPWeaponMgr::UPPWeaponMgr()
{
}

void UPPWeaponMgr::PPInitComponent()
{
	Super::PPInitComponent();
}

void UPPWeaponMgr::ChangeControllerRole()
{
	Super::ChangeControllerRole();

	if (IsValid(OwnerPawn))
	{
		UPPInputBindComp* tComp = Cast<UPPInputBindComp>(OwnerPawn->GetComponentByClass(UPPInputBindComp::StaticClass()));
		if (IsValid(tComp))
		{
			tComp->OnFire.AddUniqueDynamic(this, &UPPWeaponMgr::OnFire);
		}
	}
}

void UPPWeaponMgr::OnFire(bool Op)
{
	if (IsValid(OwnerPawn))
	{
		EPPCustomAction tAction = Op ? EPPCustomAction::Fire : EPPCustomAction::None;
		OwnerPawn->SetCustomAction(tAction);
	}
}
