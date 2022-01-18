// Fill out your copyright notice in the Description page of Project Settings.


#include "PPWeaponBase.h"

#include "PPGame/Framework/PPCharacter.h"


APPWeaponBase::APPWeaponBase()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

	SetReplicates(true);
}

void APPWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	OwnerPawn = Cast<APPCharacter>(GetOwner());
}

void APPWeaponBase::Equip()
{
	
}
