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
	virtual void BeginPlay() override;
	virtual void PPInitComponent() override;
	virtual void ChangeControllerRole() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<APPWeaponBase> > WeaponClassList;
	
	UPROPERTY(Replicated)
	TArray<APPWeaponBase*> WeaponList;
	
	UFUNCTION()
	void OnRep_CurrWeapon(APPWeaponBase* PreWeapon);
	UPROPERTY(ReplicatedUsing=OnRep_CurrWeapon)
	APPWeaponBase* CurrWeapon;
	int CurrIndex;
	
	void Equip(APPWeaponBase* NewWeapon, bool Force);
	void OnWeaponChanged(APPWeaponBase* PreWeapon);
	UFUNCTION(Server, Reliable)
	void Server_Equip(APPWeaponBase* NewWeapon, bool Force);
	
public:
	UFUNCTION()
	void OnFire(bool Op);

	UFUNCTION()
	void OnSwitchWeapon(bool Up);
	
};
