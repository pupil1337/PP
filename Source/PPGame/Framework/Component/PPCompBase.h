// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PPCompBase.generated.h"

UENUM()
enum class EPPCompSpawnCondition: uint8
{
	EPPCSC_Multi                 = 0,
	EPPCSC_OnlyOnClients         = 1,
	EPPCSC_OnlyOnServer          = 2,
	EPPCSC_OnlyOnAutonomousProxy = 3,
	EPPCSC_OnlyOnSimulatedProxy  = 4,
};

UCLASS(Abstract)
class PPGAME_API UPPCompBase : public UActorComponent
{
	GENERATED_BODY()

public:
	UPPCompBase();
	virtual void BeginPlay() override;

	virtual void PPInitComponent(){ }

	//==============================================================//
	UPROPERTY(EditDefaultsOnly, Category="PP")
	EPPCompSpawnCondition CompSpawnCondition;
};
