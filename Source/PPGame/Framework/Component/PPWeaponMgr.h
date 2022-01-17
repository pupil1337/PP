// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPCompBase.h"
#include "PPWeaponMgr.generated.h"

class APPWeaponBase;

/**
 * 
 */
UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PPGAME_API UPPWeaponMgr : public UPPCompBase
{
	GENERATED_BODY()
public:
	UPPWeaponMgr();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PPInitComponent() override;
	virtual void ChangeControllerRole() override;
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<APPWeaponBase> > WeaponClassList;

	UPROPERTY(Replicated)
	APPWeaponBase* WeaponList[10];
	
	UFUNCTION()
	void OnRep_CurrWeapon(APPWeaponBase* PreWeapon);
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_CurrWeapon)
	APPWeaponBase* CurrWeapon;

	void Equip(APPWeaponBase* NewWeapon, bool Force);
	void OnWeaponChanged(const APPWeaponBase* PreWeapon);
	void Server_Equip(APPWeaponBase* NewWeapon, bool Force);
	
	FTimerHandle InitRefreshWeaponHandle;
	UFUNCTION()
	void AutonomousInitWeapon();
	
public:
	UFUNCTION()
	void OnFire(bool Op);
	
};
