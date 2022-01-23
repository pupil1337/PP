// Fill out your copyright notice in the Description page of Project Settings.


#include "PPAttributeComp.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "PPGame/Framework/PPCharacter.h"

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
	
}

void UPPAttributeComp::CalcDamage(AActor* Instigator, float& OutDamage)
{
}

void UPPAttributeComp::TakeDamage(AActor* Instigator, float Damage)
{
	if (Health > 0)
	{
		Health -= Damage;
		if (Health <= 0)
		{
			Health = 0;
			Dead(Instigator);
		}
	}
}

void UPPAttributeComp::Dead(AActor* Instigator)
{
	OwnerPawn->GetCharacterMovement()->DisableMovement();
	Multi_Dead(Instigator);
	Client_Dead(Instigator);
}

void UPPAttributeComp::Client_Dead_Implementation(AActor* Instigator)
{
	OnDead.Broadcast();
}

void UPPAttributeComp::Multi_Dead_Implementation(AActor* Instigator)
{
	if (IsValid(DeadMontage))
	{
		OwnerPawn->PlayAnimMontage(DeadMontage, 0.3f);
		OwnerPawn->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
