// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCompBase.h"
#include "PPGame/Framework/PPCharacter.h"


UPPCompBase::UPPCompBase()
{
	CompSpawnCondition = EPPCompSpawnCondition::EPPCSC_OnlyOnServer;
}

void UPPCompBase::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerPawn = Cast<APPCharacter>(GetOwner());
}
