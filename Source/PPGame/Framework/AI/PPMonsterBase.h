// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PPMonsterBase.generated.h"

class APPCharacter;
class UAnimMontage;

USTRUCT()
struct FMonsterSkill
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float Damage = 10.0f;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* Montage = nullptr;
	
};

UCLASS()
class PPGAME_API APPMonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	APPMonsterBase();

	virtual void TakeDamage(float InDamage, AActor* instigator);
	
	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayAnimMontage(UAnimMontage* Montage);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_CollisionDisable();

	FORCEINLINE const TArray<FMonsterSkill>& GetMonsterSkills() { return MonsterSkills; }
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly)
	float HealthMax = 300.0f;
	UPROPERTY()
	float Health;

	UPROPERTY(EditDefaultsOnly)
	TArray<FMonsterSkill> MonsterSkills;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DeadMontage;

	UFUNCTION()
	void SetEnemy(APPCharacter* InEnemy);
	
	UFUNCTION()
	void Dead();
	
private:
	UPROPERTY()
	APPCharacter* Enemy = nullptr;

	bool bCanTakeDamage = true;
	
};
