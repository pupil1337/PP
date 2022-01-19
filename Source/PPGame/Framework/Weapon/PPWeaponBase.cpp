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
	AttachToComponent(OwnerPawn->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Socket_Weapon"));

	if (IsValid(OwnerPawn) && OwnerPawn->IsLocallyControlled())
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
		UGameplayStatics::SpawnEmitterAttached(MuzzlePS, Mesh, MUZZLE);
	}
}

FVector APPWeaponBase::GetMuzzleLocation()
{
	return Mesh->GetSocketLocation(MUZZLE);
}
