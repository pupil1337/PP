// Fill out your copyright notice in the Description page of Project Settings.


#include "PPAttributeComp.h"

#include "PPUIMgr.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
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

	DOREPLIFETIME_CONDITION(UPPAttributeComp, Health, COND_AutonomousOnly);
}

void UPPAttributeComp::OnRep_Health(float PreHealth)
{
	if (IsValid(OwnerPawn))
	{
		UPPUIMgr* tComp = Cast<UPPUIMgr>(OwnerPawn->GetComponentByClass(UPPUIMgr::StaticClass()));
		if (IsValid(tComp))
		{
			tComp->OnHealth(Health);
		}
	}
}

void UPPAttributeComp::CalcDamage(AActor* Instigator, float& OutDamage)
{
}

void UPPAttributeComp::TakeDamage(AActor* Instigator, float Damage)
{
	if (Health > 0)
	{
		float PreHealth = Health;
		Health = FMath::Clamp(PreHealth - Damage, 0.0f, HealthMax);
		if (OwnerPawn->IsLocallyControlled())
		{
			OnRep_Health(PreHealth);
		}
		if (Health == 0)
		{
			Dead(Instigator);
		}
	}
}

void UPPAttributeComp::Dead(AActor* Instigator)
{
	OwnerPawn->GetCharacterMovement()->DisableMovement();
	Multi_Dead(Instigator);
	Client_Dead(Instigator);

	FTimerHandle tHandle;
	GetWorld()->GetTimerManager().SetTimer(tHandle, this, &UPPAttributeComp::ReSpawn, 3.0f);
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

void UPPAttributeComp::ReSpawn()
{
	Health = HealthMax;
	OwnerPawn->GetCharacterMovement()->SetDefaultMovementMode();
	OwnerPawn->SetActorHiddenInGame(false);
	Multi_ReSpawn();
	Client_ReSpawn();

	TArray<AActor*> tFoundArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), tFoundArray);
	if (tFoundArray.Num() > 0)
	{
		APlayerStart* tStart = Cast<APlayerStart>(tFoundArray[0]);
		OwnerPawn->SetActorLocationAndRotation(tStart->GetActorLocation(), tStart->GetActorRotation());
	}
}

void UPPAttributeComp::Client_ReSpawn_Implementation()
{
	OnReSpawn.Broadcast();
}

void UPPAttributeComp::Multi_ReSpawn_Implementation()
{
	if (IsValid(DeadMontage))
	{
		OwnerPawn->StopAnimMontage();
		OwnerPawn->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}
