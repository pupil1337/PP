// Fill out your copyright notice in the Description page of Project Settings.


#include "PPWeaponMgr.h"

#include "PPAttributeComp.h"
#include "PPInputBindComp.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PPGame/Framework/PPCharacter.h"
#include "PPGame/Framework/Weapon/PPWeaponBase.h"
#include "PPGame/Framework/Projectile/PPProjectileBase.h"
#include "PPGame/Framework/Weapon/PPWeaponLauncherBase.h"


UPPWeaponMgr::UPPWeaponMgr()
{
	CompSpawnCondition = EPPCompSpawnCondition::EPPCSC_Multi;
}

void UPPWeaponMgr::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UPPWeaponMgr, CurrWeapon, COND_SimulatedOnly);
	DOREPLIFETIME_CONDITION(UPPWeaponMgr, WeaponList, COND_AutonomousOnly);
	DOREPLIFETIME_CONDITION(UPPWeaponMgr, WeaponClassList, COND_AutonomousOnly);
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
		static FTimerHandle tHandle;
		GetWorld()->GetTimerManager().SetTimer(tHandle, FTimerDelegate::CreateLambda([&]()
		{
			if (WeaponList.Num() > 0 && IsValid(WeaponList[0]))
			{
				Equip(WeaponList[0], true);
				GetWorld()->GetTimerManager().ClearTimer(tHandle);
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
		UPPInputBindComp* tInputBindComp = Cast<UPPInputBindComp>(OwnerPawn->GetComponentByClass(UPPInputBindComp::StaticClass()));
		if (IsValid(tInputBindComp))
		{
			tInputBindComp->OnFire.AddUniqueDynamic(this, &UPPWeaponMgr::OnFireState);
			tInputBindComp->OnReload.AddUniqueDynamic(this, &UPPWeaponMgr::OnReload);
			tInputBindComp->OnAim.AddUniqueDynamic(this, &UPPWeaponMgr::OnAimState);
			tInputBindComp->OnChangeWeapon.AddUniqueDynamic(this, &UPPWeaponMgr::OnSwitchWeapon);
			tInputBindComp->OnInteract.AddUniqueDynamic(this, &UPPWeaponMgr::OnPickup);
		}

		UPPAttributeComp* tAttrComp = Cast<UPPAttributeComp>(OwnerPawn->GetComponentByClass(UPPAttributeComp::StaticClass()));
		if (IsValid(tAttrComp))
		{
			tAttrComp->OnDead.AddUniqueDynamic(this, &UPPWeaponMgr::OnDead);
			tAttrComp->OnReSpawn.AddUniqueDynamic(this, &UPPWeaponMgr::OnReSpawn);
		}
	}
}

void UPPWeaponMgr::OnUnregister()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		for (auto& it:WeaponList)
		{
			GetWorld()->DestroyActor(it, true);
		}
	}
	
	Super::OnUnregister();
}

void UPPWeaponMgr::Equip(APPWeaponBase* NewWeapon, bool Force)
{
	if (CurrWeapon != NewWeapon || Force)
	{
		APPWeaponBase* PreWeapon = CurrWeapon;
		CurrWeapon = NewWeapon;
		OnWeaponChanged(PreWeapon);
		
		if (GetOwnerRole() == ROLE_AutonomousProxy)
		{
			Server_Equip(CurrWeapon, Force);
		}
	}
}

void UPPWeaponMgr::OnWeaponChanged(APPWeaponBase* PreWeapon)
{
	if (IsValid(PreWeapon))
	{
		PreWeapon->UnEquip();
	}

	if (IsValid(CurrWeapon))
	{
		CurrWeapon->Equip();
		if (IsValid(OwnerPawn))
		{
			OwnerPawn->SetOverlayState(CurrWeapon->WeaponType);
			if (ChangeWeaponMontages.Contains(CurrWeapon->WeaponType))
			{
				UAnimMontage* tMontage = ChangeWeaponMontages[CurrWeapon->WeaponType];
				if (IsValid(tMontage))
				{
					OwnerPawn->PlayAnimMontage(tMontage);
				}
			}
			if (OwnerPawn->IsLocallyControlled())
			{
				if (bFiring)
				{
					OwnerPawn->SetCustomAction(EPPCustomAction::Fire);
				}
				else
				{
					OwnerPawn->SetCustomAction(EPPCustomAction::None);
				}
				bChangeClip = false;
			}
		}
	}
}

void UPPWeaponMgr::Server_Equip_Implementation(APPWeaponBase* NewWeapon, bool Force)
{
	if (IsValid(NewWeapon))
	{
		Equip(NewWeapon, Force);
	}
}

void UPPWeaponMgr::OnRep_CurrWeapon(APPWeaponBase* PreWeapon)
{
	OnWeaponChanged(PreWeapon);
}

void UPPWeaponMgr::OnSwitchWeapon(bool Up)
{
	if (Active && WeaponList.Num() > 0)
	{
		CurrIndex += Up ? -1 : 1;
        if (CurrIndex < 0)
        {
        	CurrIndex = WeaponList.Num() - 1;
        }
        else if (CurrIndex >= WeaponList.Num())
        {
        	CurrIndex = 0;
        }

		if (IsValid(WeaponList[CurrIndex]))
		{
			Equip(WeaponList[CurrIndex], false);
		}
	}
}

void UPPWeaponMgr::OnPickup()
{
	if (IsValid(OwnerPawn))
	{
		if (OwnerPawn->GetLocalRole() == ROLE_Authority)
		{
			EquipAndDele(PickItem, PickBase);
		}
		else
		{
			Server_EquipAndDele(PickItem, PickBase);
		}
	}
}

void UPPWeaponMgr::EquipAndDele(TSubclassOf<AActor> InPickItem, AActor* InPickBase)
{
	bool bSuccess = false;
	if (IsValid(InPickItem))
	{
		UClass* tClass = InPickItem.Get();
		if (IsValid(tClass))
		{
			FActorSpawnParameters tParame;
			tParame.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			tParame.Owner = OwnerPawn;
			// 武器
			if (tClass->IsChildOf<APPWeaponBase>())
			{
				if (!WeaponClassList.Contains(InPickItem))
				{
					APPWeaponBase* tWeapon = Cast<APPWeaponBase>(GetWorld()->SpawnActor<APPWeaponBase>(tClass, tParame));
					if (IsValid(tWeapon))
					{
						tWeapon->SetActorHiddenInGame(true);
						WeaponList.Add(tWeapon);
						WeaponClassList.Add((TSubclassOf<APPWeaponBase>)InPickItem);
						bSuccess = true;
					}
				}
			}
			// 炮弹
			else if (tClass->IsChildOf<APPProjectileBase>())
			{
				for (auto& it: WeaponList)
				{
					APPWeaponLauncherBase* tLauncher = Cast<APPWeaponLauncherBase>(it);
					if (IsValid(tLauncher))
					{
						tLauncher->ProjectileUpPro((TSubclassOf<APPProjectileBase>)InPickItem);
						bSuccess = true;
					}
				}
			}
		}
	}

	if (IsValid(InPickBase) && bSuccess)
	{
		OwnerPawn->GetWorld()->DestroyActor(InPickBase, true);
	}
}

void UPPWeaponMgr::Server_EquipAndDele_Implementation(TSubclassOf<AActor> InPickItem, AActor* InPickBase)
{
	EquipAndDele(InPickItem, InPickBase);
}

void UPPWeaponMgr::OnDead()
{
	Active = false;
	Server_OnDead();
}

void UPPWeaponMgr::Server_OnDead_Implementation()
{
	if (IsValid(CurrWeapon))
	{
		CurrWeapon->SetActorHiddenInGame(true);
	}
}

void UPPWeaponMgr::OnReSpawn()
{
	Active = true;
	Server_ReSpawn();
}

void UPPWeaponMgr::Server_ReSpawn_Implementation()
{
	if (IsValid(CurrWeapon))
	{
		CurrWeapon->SetActorHiddenInGame(false);
	}
}

void UPPWeaponMgr::OnFireState(bool Op)
{
	bFiring = Op;
	if (Op)
	{
		OnFire(Op);
	}
}

void UPPWeaponMgr::OnAimState(bool Op)
{
	if (Active && IsValid(CurrWeapon))
	{
		CurrWeapon->Aim(Op);
	}
}

void UPPWeaponMgr::OnFire(bool Op)
{
	if (Active && IsValid(OwnerPawn) && IsValid(CurrWeapon))
	{
		if (Op == true)
		{
			if (!bChangeClip)
			{
				CurrWeapon->Fire(true);
			}
		}
		else
		{
			if (bFiring)
			{
				OnFire(true);
			}
			else if(!bChangeClip)
			{
				CurrWeapon->Fire(false);
			}
		}
	}
}

void UPPWeaponMgr::OnReload(bool Start)
{
	if (Active &&  IsValid(CurrWeapon) && (!Start || !bChangeClip))
	{
		bChangeClip = CurrWeapon->Reload(Start);
	}
}

void UPPWeaponMgr::OnReloadEnd()
{
	if (bFiring)
	{
		OnFire(true);
	}
	else
	{
		OwnerPawn->SetCustomAction(EPPCustomAction::None);
	}
}
