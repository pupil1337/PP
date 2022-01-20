// Fill out your copyright notice in the Description page of Project Settings.


#include "PPWeaponBase.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PPGame/Framework/PPCharacter.h"
#include "PPGame/Framework/UI/PPCrosshairWidget.h"

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

bool APPWeaponBase::Fire()
{
	if (IsValid(OwnerPawn))
	{
		APlayerController* tPC = Cast<APlayerController>(OwnerPawn->GetController());
		if (IsValid(tPC) && IsValid(tPC->PlayerCameraManager))
		{
			CurrFireInfo.CameraLocation = tPC->PlayerCameraManager->GetCameraLocation();
			CurrFireInfo.CameraRotation = tPC->PlayerCameraManager->GetCameraRotation();
			CurrFireInfo.MuzzleLocation = GetMuzzleLocation();
		}
	}
	return true;
}

void APPWeaponBase::PlayMuzzlePS()
{
	if (IsValid(MuzzlePS))
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzlePS, Mesh, MUZZLE, {}, {}, EAttachLocation::SnapToTarget);
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
	Result.FireRotation = Mesh->GetSocketRotation(MUZZLE); Result.FireRotation.Normalize();
	Result.HitLocation = tEnd;
	
	FCollisionQueryParams tParams;
	tParams.AddIgnoredActor(OwnerPawn);
	FHitResult tHitResult;
	bool bHit = GetWorld()->LineTraceSingleByChannel(tHitResult, tStart, tEnd, ECollisionChannel::ECC_Visibility, tParams);
	if (bHit)
	{
		tEnd = tHitResult.Location;
		FVector tFireForward = tEnd - tStart; tFireForward.Normalize();
		if (FVector::DotProduct(tForward, tFireForward) > 0.0f)
		{
			Result.FireRotation = tFireForward.Rotation(); Result.FireRotation.Normalize();
			Result.HitLocation = tEnd;
			Result.HitActor = tHitResult.GetActor();
		}
	}
}
