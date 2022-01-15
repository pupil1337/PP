// Fill out your copyright notice in the Description page of Project Settings.


#include "PPAttributeComp.h"

UPPAttributeComp::UPPAttributeComp()
{
	CompSpawnCondition = EPPCompSpawnCondition::EPPCSC_OnlyOnAutonomousProxy;
}

void UPPAttributeComp::BeginPlay()
{
	Super::BeginPlay();
	
}
