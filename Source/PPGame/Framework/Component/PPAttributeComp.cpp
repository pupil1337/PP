// Fill out your copyright notice in the Description page of Project Settings.


#include "PPAttributeComp.h"

#include "Net/UnrealNetwork.h"

UPPAttributeComp::UPPAttributeComp()
{
	CompSpawnCondition = EPPCompSpawnCondition::EPPCSC_Multi;
}

void UPPAttributeComp::BeginPlay()
{
	Super::BeginPlay();

	Health = HealthMax;
}

void UPPAttributeComp::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UPPAttributeComp, Health, COND_SimulatedOnly);
}

void UPPAttributeComp::OnRep_Health(float PreHealth)
{
	int a = 1;
}

void UPPAttributeComp::CalcDamage(AActor* Instigator, float& OutDamage)
{
}

void UPPAttributeComp::TakeDamage(AActor* Instigator, float Damage)
{
	Health -= Damage;
}
