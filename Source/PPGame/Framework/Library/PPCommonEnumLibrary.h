// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPCommonEnumLibrary.generated.h"


UENUM(BlueprintType)
enum class EPPDamageType : uint8
{
	Normal,
	Electric,
	Poison,
	Fire,
	Explod
};
