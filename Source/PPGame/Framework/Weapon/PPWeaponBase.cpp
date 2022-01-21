// Fill out your copyright notice in the Description page of Project Settings.


#include "PPWeaponBase.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PPGame/Framework/PPCharacter.h"
#include "PPGame/Framework/Component/PPAttributeComp.h"
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

void APPWeaponBase::Reload()
{
	if (IsValid(OwnerPawn))
	{
		OwnerPawn->SetCustomAction(EPPCustomAction::ChangeClip);
	}
}

bool APPWeaponBase::Fire(bool Op)
{
	if (IsValid(OwnerPawn))
	{
		if (Op == true)
		{
			if (CurrClipSize == 0)
			{
				Reload();
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

void APPWeaponBase::PlayMuzzlePS()
{
	if (IsValid(MuzzlePS))
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzlePS, Mesh, MUZZLE, {}, {}, EAttachLocation::SnapToTarget);
	}
}

void APPWeaponBase::PlayFireSound()
{
	if (IsValid(FireSoundCue))
	{
		UGameplayStatics::SpawnSoundAttached(FireSoundCue, Mesh, MUZZLE);
	}
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

void APPWeaponBase::TakeDamage(AActor* Victim)
{
	if (IsValid(Victim))
	{
		if (OwnerPawn->GetLocalRole() == ROLE_AutonomousProxy)
		{
			Server_TakeDamage(Victim);
		}
		else if (OwnerPawn->GetLocalRole() == ROLE_Authority)
		{
			APPCharacter* tPlayer = Cast<APPCharacter>(Victim);
			if (IsValid(tPlayer))
			{
				UPPAttributeComp* tComp = Cast<UPPAttributeComp>(tPlayer->GetComponentByClass(UPPAttributeComp::StaticClass()));
				if (IsValid(tComp))
				{
					tComp->TakeDamage(OwnerPawn, WeaponCfg.Damage);	
				}
			}
		}
	}
}

void APPWeaponBase::Server_TakeDamage_Implementation(AActor* Victim)
{
	TakeDamage(Victim);
}
