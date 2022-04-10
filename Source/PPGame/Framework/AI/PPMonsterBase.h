// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PPGame/Framework/Library/PPCommonEnumLibrary.h"
#include "PPMonsterBase.generated.h"

class APPCharacter;
class UAnimMontage;
class UBlackboardComponent;
class UParticleSystem;

USTRUCT()
struct FMonsterSkill
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	float Damage = 10.0f;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* Montage = nullptr;

	UPROPERTY(EditDefaultsOnly)
	int Weight = 1;
	
};

UCLASS()
class PPGAME_API APPMonsterBase : public ACharacter
{
	GENERATED_BODY()

public:
	APPMonsterBase();

	virtual void MonsterTakeDamage(float InDamage, AActor* tInstigator);
	UBlackboardComponent* GetBlackboardComp();
	
	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayAnimMontage(UAnimMontage* Montage);

	UFUNCTION(NetMulticast, Reliable)
	void Multi_CollisionDisable();

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_PlayParticleSystem(UParticleSystem* ParticleSystem, FVector Location);

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_PlayDecal(UMaterialInterface* DecalMat, FVector Size, FVector Location, float Life);

	UFUNCTION(NetMulticast, Unreliable)
	void Multi_SetDamageDeBuff(EPPDamageType DamageType);

	FORCEINLINE const TArray<FMonsterSkill>& GetMonsterSkills() { return MonsterSkills; }
	FORCEINLINE APPCharacter* GetEnemy() { return Enemy; }

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
	UPROPERTY(EditDefaultsOnly)
	TMap<EPPDamageType, UParticleSystem*> DeBuffEffects;
	UPROPERTY(EditDefaultsOnly)
	FName DeBuffSocket;

	virtual void SetEnemy(APPCharacter* InEnemy);
	void Dead(AActor* tInstigator);
	bool MonsterIsAlive();
	
private:
	UPROPERTY()
	APPCharacter* Enemy = nullptr;

	bool bCanTakeDamage = true;

	UPROPERTY()
	TMap<EPPDamageType, UParticleSystemComponent*> DeBuffPSMap;
	
};
