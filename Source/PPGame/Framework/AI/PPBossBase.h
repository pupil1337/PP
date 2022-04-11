// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPMonsterBase.h"
#include "PPBossBase.generated.h"

class APPCharacter;

/**
 * 
 */
UCLASS()
class PPGAME_API APPBossBase : public APPMonsterBase
{
	GENERATED_BODY()

public:
	APPBossBase();
	
	virtual void Tick(float DeltaSeconds);

protected:
	virtual void SetEnemy(APPCharacter* InEnemy) override;
	virtual void Dead(AActor* tInstigator) override;
};
