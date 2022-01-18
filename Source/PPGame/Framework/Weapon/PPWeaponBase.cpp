// Fill out your copyright notice in the Description page of Project Settings.


#include "PPWeaponBase.h"

#include "PPGame/Framework/PPCharacter.h"


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
}

void APPWeaponBase::Equip()
{
	SetActorHiddenInGame(false);
}

void APPWeaponBase::UnEquip()
{
	SetActorHiddenInGame(true);
}
