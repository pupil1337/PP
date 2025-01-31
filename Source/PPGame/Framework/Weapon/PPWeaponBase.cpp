﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "PPWeaponBase.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PPGame/Framework/PPCharacter.h"
#include "PPGame/Framework/Component/PPUIMgr.h"
#include "PPGame/Framework/Component/PPWeaponMgr.h"
#include "PPGame/Framework/UI/PPCrosshairWidget.h"
#include "Sound/SoundCue.h"

const FName MUZZLE(TEXT("MuzzleFlashSocket"));

APPWeaponBase::APPWeaponBase()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

	bReplicates = true;
}

void APPWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerPawn = Cast<APPCharacter>(GetOwner());
	if (IsValid(OwnerPawn) && OwnerPawn->GetLocalRole() != ROLE_SimulatedProxy)
	{
		AttachToComponent(OwnerPawn->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Socket_Weapon"));

		if (OwnerPawn->IsLocallyControlled())
		{
			if (IsValid(CrosshairWidgetClass))
			{
				Crosshair = Cast<UPPCrosshairWidget>(CreateWidget(GetWorld(), CrosshairWidgetClass));
				if (IsValid(Crosshair))
				{
					Crosshair->AddToViewport(0);
				}
			}
			CurrClipSize = WeaponCfg.ClipSize;
			SpareClipSize = WeaponCfg.SpareSize;
		}
	}
}

void APPWeaponBase::Equip()
{
	SetActorHiddenInGame(false);
	OnWeaponEvent();
}

void APPWeaponBase::UnEquip()
{
	SetActorHiddenInGame(true);
}

void APPWeaponBase::Aim(bool Op)
{
	if (IsValid(Crosshair))
	{
		Crosshair->Aim(Op);
	}
}

bool APPWeaponBase::Reload(bool Start)
{
	if (IsValid(OwnerPawn))
	{
		if (Start)
		{
			if (CurrClipSize < WeaponCfg.ClipSize)
			{
				OwnerPawn->SetCustomAction(EPPCustomAction::ChangeClip);
				return true;
			}
		}
		else
		{
			if (CurrClipSize < WeaponCfg.ClipSize)
			{
				CurrClipSize += SpareClipSize;
				SpareClipSize = 0;
				int tOver = CurrClipSize - WeaponCfg.ClipSize;
				if (tOver > 0)
				{
					CurrClipSize = WeaponCfg.ClipSize;
					SpareClipSize = tOver;
				}
				OnWeaponEvent();
			}

			UPPWeaponMgr* tComp = Cast<UPPWeaponMgr>(OwnerPawn->GetComponentByClass(UPPWeaponMgr::StaticClass()));
			if (IsValid(tComp))
			{
				tComp->OnReloadEnd();
			}
		}
	}
	return false;
}

bool APPWeaponBase::Fire(bool Op)
{
	if (IsValid(OwnerPawn))
	{
		if (Op == true)
		{
			if (CurrClipSize == 0)
			{
				Reload(true);
			}
			else
			{
				APlayerController* tPC = Cast<APlayerController>(OwnerPawn->GetController());
				if (IsValid(tPC) && IsValid(tPC->PlayerCameraManager))
				{
					CurrFireInfo.CameraLocation = tPC->PlayerCameraManager->GetCameraLocation();
					CurrFireInfo.CameraRotation = tPC->PlayerCameraManager->GetCameraRotation();
					CurrFireInfo.MuzzleLocation = Mesh->GetSocketLocation(MUZZLE);
					CurrFireInfo.MuzzleRotation = Mesh->GetSocketRotation(MUZZLE);
				}
				--CurrClipSize;
				OwnerPawn->SetCustomAction(EPPCustomAction::Fire);
				PlayEffect();
				OnWeaponEvent();
				return true;
			}
		}
		else
		{
			OwnerPawn->SetCustomAction(EPPCustomAction::None);
		}
	}
	return false;
}

FVector APPWeaponBase::GetMuzzleLocation()
{
	return Mesh->GetSocketLocation(MUZZLE);
}

void APPWeaponBase::CalcTraceResult(FTraceResult& Result)
{
	FVector tStart = CurrFireInfo.CameraLocation;
	FVector tForward = CurrFireInfo.CameraRotation.Vector(); tForward.Normalize();
	FVector tEnd = tStart + tForward * WeaponCfg.ValidDistance;

	Result.FireLocation = CurrFireInfo.MuzzleLocation;
	
	FCollisionQueryParams tParams;
	tParams.AddIgnoredActor(OwnerPawn);
	FHitResult tHitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(tHitResult, tStart, tEnd, ECollisionChannel::ECC_Visibility, tParams);
	if (bHit)
	{
		FVector tFireForward = tHitResult.Location - Result.FireLocation; tFireForward.Normalize();
		FVector tMuzzleForward = CurrFireInfo.MuzzleRotation.Vector(); tMuzzleForward.Normalize();
		if (FVector::DotProduct(tMuzzleForward, tFireForward) > 0.0f)
		{
			Result.FireRotation = tFireForward.Rotation(); Result.FireRotation.Normalize();
			Result.HitActor = tHitResult.GetActor();
			Result.HitLocation = tHitResult.Location;
			return;
		}
	}

	Result.HitLocation = tEnd;
	FVector tNotHitForward = Result.HitLocation - Result.FireLocation; tNotHitForward.Normalize();
	Result.FireRotation = tNotHitForward.Rotation(); Result.FireRotation.Normalize();
}

void APPWeaponBase::PlayEffect()
{
	if (IsValid(MuzzlePS))
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzlePS, Mesh, MUZZLE, {}, {0.0f, -90.0f, 0.0f}, EAttachLocation::SnapToTarget);
	}

	if (IsValid(FireSoundCue))
	{
		UGameplayStatics::SpawnSoundAttached(FireSoundCue, Mesh, MUZZLE);
	}

	if (OwnerPawn->GetLocalRole() == ROLE_AutonomousProxy)
	{
		Server_PlayEffect();
	}
	else if (GetNetMode() == NM_ListenServer)
	{
		Multi_PlayEffect();
	}
}

void APPWeaponBase::OnWeaponEvent()
{
	if (IsValid(OwnerPawn) && OwnerPawn->IsLocallyControlled())
	{
		UPPUIMgr* tComp = Cast<UPPUIMgr>(OwnerPawn->GetComponentByClass(UPPUIMgr::StaticClass()));
		if (IsValid(tComp))
		{
			tComp->OnWeapon(WeaponCfg.WeaponIdex, CurrClipSize, SpareClipSize);
		}
	}
}

void APPWeaponBase::Server_PlayEffect_Implementation()
{
	Multi_PlayEffect();
}

void APPWeaponBase::Multi_PlayEffect_Implementation()
{
	if (IsValid(OwnerPawn) && OwnerPawn->GetLocalRole() == ROLE_SimulatedProxy)
	{
		PlayEffect();
	}
}
