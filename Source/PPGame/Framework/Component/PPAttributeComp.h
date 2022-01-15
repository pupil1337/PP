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
	
};
