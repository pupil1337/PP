// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPCompBase.h"
#include "PPAttributeComp.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PPGAME_API UPPAttributeComp : public UPPCompBase
{
	GENERATED_BODY()

public:
	UPPAttributeComp();

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(EditDefaultsOnly)
	float HealthMax = 100.0f;
	UPROPERTY(ReplicatedUsing=OnRep_Health)
	float Health;

	UFUNCTION()
	void OnRep_Health(float PreHealth);

public:
	UFUNCTION()
	void CalcDamage(AActor* Instigator, float& OutDamage);

	UFUNCTION()
	void TakeDamage(AActor* Instigator, float Damage);
	
};
