// Fill out your copyright notice in the Description page of Project Settings.


#include "PPWeaponMgr.h"

#include "PPInputBindComp.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PPGame/Framework/PPCharacter.h"
#include "PPGame/Framework/Weapon/PPWeaponBase.h"


UPPWeaponMgr::UPPWeaponMgr()
{
}

void UPPWeaponMgr::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UPPWeaponMgr, CurrWeapon, COND_SimulatedOnly);
	DOREPLIFETIME_CONDITION(UPPWeaponMgr, WeaponList, COND_AutonomousOnly);
}

void UPPWeaponMgr::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetOwnerRole() == ROLE_Authority)
	{
		FActorSpawnParameters tParame;
		tParame.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		tParame.Owner = OwnerPawn;

		for (auto &it: WeaponClassList)
		{
			if (IsValid(it))
			{
				UClass* tClass = it.Get();
				if (IsValid(tClass))
				{
					APPWeaponBase* tWeapon = Cast<APPWeaponBase>(GetWorld()->SpawnActor<APPWeaponBase>(tClass, tParame));
					if (IsValid(tWeapon))
					{
						tWeapon->SetActorHiddenInGame(true);
						WeaponList.Add(tWeapon);
					}
				}
			}
		}
		
		if (!IsValid(WeaponList[0]))
		{
			UE_LOG(LogTemp, Error, TEXT("没有武器怎么玩???"));
			check(false);
		}
	}
}

void UPPWeaponMgr::PPInitComponent()
{
	Super::PPInitComponent();
	
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		GetWorld()->GetTimerManager().SetTimer(AutonomousInitEquipHandle, FTimerDelegate::CreateLambda([this]()
		{
			if (WeaponList.Num() > 0 && IsValid(WeaponList[0]))
			{
				Equip(WeaponList[0], true);
				GetWorld()->GetTimerManager().ClearTimer(AutonomousInitEquipHandle);
			}
		}), 0.1f, true);
	}
	else if (GetOwnerRole() == ROLE_Authority && GetNetMode() != NM_DedicatedServer)
	{
		if (WeaponList.Num() > 0 && IsValid(WeaponList[0]))
		{
			Equip(WeaponList[0], true);
		}
	}
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

void UPPWeaponMgr::Equip(APPWeaponBase* NewWeapon, bool Force)
{
	if (CurrWeapon != NewWeapon || Force)
	{
		const APPWeaponBase* PreWeapon = CurrWeapon;
		CurrWeapon = NewWeapon;
		OnWeaponChanged(PreWeapon);
		
		if (GetOwnerRole() == ROLE_AutonomousProxy)
		{
			Server_Equip(CurrWeapon, Force);
		}
	}
}

void UPPWeaponMgr::OnWeaponChanged(const APPWeaponBase* PreWeapon)
{
	CurrWeapon->Equip();
	
	if (IsValid(OwnerPawn))
	{
		OwnerPawn->SetOverlayState(CurrWeapon->WeaponType);
	}
}

void UPPWeaponMgr::Server_Equip_Implementation(APPWeaponBase* NewWeapon, bool Force)
{
	Equip(NewWeapon, Force);
}

void UPPWeaponMgr::OnRep_CurrWeapon(APPWeaponBase* PreWeapon)
{
	OnWeaponChanged(PreWeapon);
}

void UPPWeaponMgr::OnFire(bool Op)
{
	if (IsValid(OwnerPawn))
	{
		EPPCustomAction tAction = Op ? EPPCustomAction::Fire : EPPCustomAction::None;
		OwnerPawn->SetCustomAction(tAction);
	}
}
