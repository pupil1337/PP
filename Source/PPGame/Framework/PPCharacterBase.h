// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/PPCompBase.h"
#include "GameFramework/Character.h"
#include "PPCharacterBase.generated.h"

class UPPCompBase;

UCLASS()
class PPGAME_API APPCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	APPCharacterBase();
	
protected:
	virtual void PostInitializeComponents() override;
	virtual void PostNetInit() override;
	virtual void BeginPlay() override;

	//==============================================================//
protected:
	void CreateAllComps(bool bPostNetInit);
	void CreateComp(bool bPostNetInit, TSubclassOf<UPPCompBase> ClassType);
	void CreateFilter(bool& bNeedCreate, bool& bReplicated, bool bPostNetInit, EPPCompSpawnCondition ESpawnCondition);
	void LinkAndInitAllComps();
	void LinkAndInitComp(TSubclassOf<UPPCompBase> ClassType);
	void ChangeControllerRole();
	
	UPROPERTY(EditDefaultsOnly, Category="PP|Components")
	TArray<TSubclassOf<UPPCompBase> > PPCompClasses;

private:
	bool bInitComps;
	
	UPROPERTY()
	TArray<UPPCompBase*> PPComps;
	
};
