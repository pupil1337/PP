// Fill out your copyright notice in the Description page of Project Settings.


#include "PPWeaponBase.h"


APPWeaponBase::APPWeaponBase()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

	SetReplicates(true);
}

void APPWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void APPWeaponBase::Equip()
{
	
}
