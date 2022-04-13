// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PPGLaDOS.generated.h"

class APPMonsterBase;
class APPCharacter;

UCLASS()
class PPGAME_API APPGLaDOS : public AActor
{
	GENERATED_BODY()
	
public:
	APPGLaDOS();

	void OnMonsterSpawn(TSubclassOf<APPMonsterBase> MonsterClass, FVector Location, APPCharacter* TriggerPlayer = nullptr);
	void OnMonsterDeath(APPMonsterBase* Monster);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TArray<APPMonsterBase*> AllMonsters;
};
